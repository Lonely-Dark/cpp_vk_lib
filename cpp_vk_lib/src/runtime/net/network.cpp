#include "cpp_vk_lib/runtime/net/network.hpp"

#include "cpp_vk_lib/runtime/misc/cppdefs.hpp"
#include "spdlog/spdlog.h"

#include <curl/curl.h>
#include <thread>

bool cpp_vk_lib_curl_verbose;
static pthread_mutex_t share_data_lock[CURL_LOCK_DATA_LAST];
static CURLSH* shared_handle = curl_share_init();
static std::unordered_map<std::thread::id, CURL*> curl_handles;

static void libcurl_lock_cb(CURL* handle, curl_lock_data data, curl_lock_access access, void* stream) noexcept
{
    VK_UNUSED(handle);
    VK_UNUSED(access);
    VK_UNUSED(stream);
    pthread_mutex_lock(&share_data_lock[data]);
}

static void libcurl_unlock_cb(CURL* handle, curl_lock_data data, void* stream) noexcept
{
    VK_UNUSED(handle);
    VK_UNUSED(stream);
    pthread_mutex_unlock(&share_data_lock[data]);
}

static size_t libcurl_omit_string_cb(char* contents, size_t size, size_t nmemb, void* stream) noexcept
{
    VK_UNUSED(contents);
    VK_UNUSED(stream);
    return size * nmemb;
}

static size_t libcurl_string_write_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    static_cast<std::string*>(stream)->append(contents, size * nmemb);
    return size * nmemb;
}

static size_t libcurl_buffer_write_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    auto vector = static_cast<std::vector<uint8_t>*>(stream);
    std::copy(contents, contents + (size * nmemb), std::back_inserter(*vector));
    return size * nmemb;
}

static size_t libcurl_file_write_cb(char* contents, size_t size, size_t nmemb, void* stream) noexcept
{
    return fwrite(contents, size, nmemb, static_cast<FILE*>(stream));
}

static size_t libcurl_string_header_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    if (!stream) {
        return size * nmemb;
    }
    size_t bytes = 0;
    sscanf(contents, "content-length: %zu\n", &bytes);
    if (bytes > 0) {
        static_cast<std::string*>(stream)->reserve(bytes);
    }
    return size * nmemb;
}

static size_t libcurl_buffer_header_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    if (!stream) {
        return size * nmemb;
    }
    size_t bytes = 0;
    sscanf(contents, "content-length: %zu\n", &bytes);
    if (bytes > 0) {
        static_cast<std::vector<uint8_t>*>(stream)->reserve(bytes);
    }
    return size * nmemb;
}

[[maybe_unused]] static int atexit_handler = []() noexcept {
    std::atexit([] {
        for (const auto& [thread_id, handle] : curl_handles) {
            if (handle) {
                curl_easy_cleanup(handle);
            }
        }
        curl_share_cleanup(shared_handle);
    });
    return 0;
}();

[[maybe_unused]] static int shared_curl_handler = []() noexcept {
    for (auto& i : share_data_lock) {
        pthread_mutex_init(&i, nullptr);
    }

    shared_handle = curl_share_init();
    curl_share_setopt(shared_handle, CURLSHOPT_LOCKFUNC, libcurl_lock_cb);
    curl_share_setopt(shared_handle, CURLSHOPT_UNLOCKFUNC, libcurl_unlock_cb);
    curl_share_setopt(shared_handle, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(shared_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(shared_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
    curl_share_setopt(shared_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    return 0;
}();

template <typename Body>
static std::string create_url(std::string_view host, Body&& body)
{
    const size_t estimated_params_length =
        /* average word length */ 20 *
        /* param, value */ 2 * body.size();
    std::string result;
    result.reserve(host.size() + estimated_params_length);
    result += host;

    auto escape = [](std::string_view url) {
        char* encoded = curl_easy_escape(nullptr, url.data(), url.length());
        std::string res(encoded);
        curl_free(encoded);
        return res;
    };

    for (const auto& [key, value] : body) {
        result += key;
        result += '=';
        result += escape(value);
        result += '&';
    }

    if (!body.empty()) {
        result.pop_back();
    }
    result.shrink_to_fit();
    return result;
}

static void libcurl_set_optional_verbose(CURL* handle) noexcept
{
    if (cpp_vk_lib_curl_verbose) {
        curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    }
}

static CURL* libcurl_create_handle(std::string_view url) noexcept
{
    auto current_thread_handle = []() noexcept {
        const std::thread::id thread_id = std::this_thread::get_id();
        if (curl_handles.find(thread_id) == curl_handles.end()) {
            curl_handles[thread_id] = curl_easy_init();
        }
        return curl_handles[thread_id];
    };

    CURL* handle = current_thread_handle();
    if (!handle) {
        spdlog::error("curl_easy_init() failed");
        exit(-1);
    }
    curl_easy_setopt(handle, CURLOPT_URL, url.data());
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 600L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "cpp_vk_lib libcurl-agent/1.1");
    curl_easy_setopt(handle, CURLOPT_SHARE, shared_handle);
    libcurl_set_optional_verbose(handle);
    return handle;
}

static runtime::result<std::string, size_t> libcurl_to_string_recv(CURL* handle, bool output_needed)
{
    std::string output;
    if (output_needed) {
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, libcurl_string_write_cb);
        curl_easy_setopt(handle, CURLOPT_HEADERDATA, &output);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, libcurl_string_header_cb);
    } else {
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, libcurl_omit_string_cb);
    }
    if (const CURLcode res = curl_easy_perform(handle); res != CURLE_OK) {
        spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
        curl_easy_reset(handle);
        return {"", -1UL};
    }
    curl_easy_reset(handle);
    output.shrink_to_fit();
    return output;
}

static size_t libcurl_to_buffer_recv(
    void* buffer, std::string_view server, curl_write_callback write_cb,
    curl_write_callback header_cb = nullptr) noexcept
{
    CURL* handle = libcurl_create_handle(server);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, buffer);
    if (header_cb) {
        curl_easy_setopt(handle, CURLOPT_HEADERDATA, buffer);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_cb);
    }
    if (const CURLcode res = curl_easy_perform(handle); res != CURLE_OK) {
        spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
        curl_easy_reset(handle);
        return -1;
    }
    curl_easy_reset(handle);
    return 0;
}

static runtime::result<std::string, size_t>
    libcurl_from_buffer_send(bool output_needed, struct curl_httppost* formpost, std::string_view server)
{
    CURL* handle = libcurl_create_handle(server);
    curl_easy_setopt(handle, CURLOPT_HTTPPOST, formpost);
    auto result = libcurl_to_string_recv(handle, output_needed);
    curl_formfree(formpost);
    return result;
}

namespace runtime {

result<std::string, size_t>
    network::request(bool output_needed, std::string_view host, const std::map<std::string, std::string>& target)
{
    const std::string url = create_url(host, target);
    return libcurl_to_string_recv(libcurl_create_handle(url), output_needed);
}

result<std::string, size_t>
    network::request(bool output_needed, std::string_view host, std::map<std::string, std::string>&& target)
{
    const std::string url = create_url(host, std::move(target));
    return libcurl_to_string_recv(libcurl_create_handle(url), output_needed);
}

result<std::string, size_t> network::request_data(bool output_needed, std::string_view host, std::string_view data)
{
    CURL* handle = libcurl_create_handle(host);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, data.data());
    return libcurl_to_string_recv(handle, output_needed);
}

size_t network::download(std::string_view filename, std::string_view server)
{
    std::unique_ptr<FILE, decltype(&fclose)> fp{fopen(filename.data(), "wb"), fclose};
    if (!fp) {
        spdlog::error("libcurl: failed to open file {}", filename);
        return -1;
    }
    return libcurl_to_buffer_recv(fp.get(), server, libcurl_file_write_cb);
}

size_t network::download(std::vector<uint8_t>& buffer, std::string_view server)
{
    buffer.clear();
    return libcurl_to_buffer_recv(&buffer, server, libcurl_buffer_write_cb, libcurl_buffer_header_cb);
}

result<std::string, size_t> network::upload(
    bool output_needed, std::string_view field_name, std::string_view filename, std::string_view server,
    std::string_view content_type)
{
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr  = nullptr;
    // clang-format off
    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME,     field_name.data(),
        CURLFORM_FILENAME,     filename.data(),
        CURLFORM_FILE,         filename.data(),
        CURLFORM_CONTENTTYPE,  content_type.data(),
        CURLFORM_END);
    // clang-format on
    return libcurl_from_buffer_send(output_needed, formpost, server);
}

result<std::string, size_t> network::upload(
    bool output_needed, const std::vector<uint8_t>& buffer, std::string_view field_name, std::string_view server,
    std::string_view content_type)
{
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr  = nullptr;
    // clang-format off
    curl_formadd(&formpost, &lastptr,
        CURLFORM_BUFFER,          "temp",
        CURLFORM_PTRNAME,         "ptr",
        CURLFORM_COPYNAME,        field_name,
        CURLFORM_BUFFERPTR,       buffer.data(),
        CURLFORM_BUFFERLENGTH,    buffer.size(),
        CURLFORM_CONTENTSLENGTH,  buffer.size(),
        CURLFORM_CONTENTTYPE,     content_type.data(),
        CURLFORM_END);
    // clang-format on
    return libcurl_from_buffer_send(output_needed, formpost, server);
}

}// namespace runtime