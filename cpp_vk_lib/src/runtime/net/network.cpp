#include "cpp_vk_lib/runtime/net/network.hpp"

#include "spdlog/spdlog.h"

#include <curl/curl.h>

bool cpp_vk_lib_curl_verbose;

static pthread_mutex_t share_datalock[CURL_LOCK_DATA_LAST];
static CURLSH* shared_handle = curl_share_init();

static std::string escape(std::string_view url)
{
    char* encoded = curl_easy_escape(NULL, url.data(), url.length());
    std::string res{encoded};
    curl_free(encoded);
    return res;
}

static void lock_cb(
    CURL* handle,
    curl_lock_data data,
    curl_lock_access access,
    void* userptr)
{
    (void)access; /* unused */
    (void)userptr; /* unused */
    (void)handle; /* unused */
    pthread_mutex_lock(&share_datalock[data]);
}

static void unlock_cb(CURL* handle, curl_lock_data data, void* userptr)
{
    (void)userptr; /* unused */
    (void)handle; /* unused */
    pthread_mutex_unlock(&share_datalock[data]);
}

void runtime::network::init_shared_curl()
{
    for (int i = 0; i < CURL_LOCK_DATA_LAST; i++)
        pthread_mutex_init(&share_datalock[i], NULL);

    shared_handle = curl_share_init();
    curl_share_setopt(shared_handle, CURLSHOPT_LOCKFUNC, lock_cb);
    curl_share_setopt(shared_handle, CURLSHOPT_UNLOCKFUNC, unlock_cb);

    curl_share_setopt(shared_handle, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(shared_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(shared_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
    curl_share_setopt(
        shared_handle,
        CURLSHOPT_SHARE,
        CURL_LOCK_DATA_SSL_SESSION);
}

static size_t
    omit_data_callback(char* contents, size_t size, size_t nmemb, void* userp)
{
    (void)contents;
    (void)userp;
    (void)size;
    (void)nmemb;
    return size * nmemb;
}

static size_t
    string_callback(char* contents, size_t size, size_t nmemb, void* userp)
{
    static_cast<std::string*>(userp)->append(contents, size * nmemb);
    return size * nmemb;
}

static size_t
    file_callback(char* contents, size_t size, size_t nmemb, void* stream)
{
    return fwrite(contents, size, nmemb, static_cast<FILE*>(stream));
}

static size_t
    buffer_callback(char* contents, size_t size, size_t nmemb, void* userp)
{
    auto vector = static_cast<std::vector<uint8_t>*>(userp);
    std::copy(contents, contents + (size * nmemb), std::back_inserter(*vector));
    return size * nmemb;
}

static size_t string_header_callback(
    char* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    if (!userp) {
        return size * nmemb;
    }
    size_t bytes = 0;
    sscanf(contents, "content-length: %zu\n", &bytes);
    if (bytes > 0) {
        static_cast<std::string*>(userp)->reserve(bytes);
    }
    return size * nmemb;
}

static size_t buffer_header_callback(
    char* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    if (!userp) {
        return size * nmemb;
    }
    size_t bytes = 0;
    sscanf(contents, "content-length: %zu\n", &bytes);
    if (bytes > 0) {
        static_cast<std::vector<uint8_t>*>(userp)->reserve(bytes);
        spdlog::trace(
            "libcurl buffer header callback called, reserve {} bytes for buffer",
            bytes);
    }
    return size * nmemb;
}

static std::string
    create_url(std::string_view host, std::map<std::string, std::string>&& body)
{
    static constexpr size_t average_word_length = 20;
    const size_t estimated_params_length =
        average_word_length * body.size() * 2;
    std::string result;
    result.reserve(host.size() + estimated_params_length);
    result += host;

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

static void set_optional_verbose(CURL* curl) noexcept
{
    if (cpp_vk_lib_curl_verbose) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }
}

namespace runtime {

result<std::string, size_t> network::request(
    bool output_needed,
    std::string_view host,
    std::map<std::string, std::string>&& target)
{
    const std::string url = create_url(host, std::move(target));
    std::unique_ptr<CURL, curl_free_callback> curl_handle{
        curl_easy_init(),
        curl_easy_cleanup};
    CURL* curl = curl_handle.get();
    if (!curl) {
        spdlog::error("curl_easy_init() failed");
        return result<std::string, size_t>(std::string(), -1);
    }
    spdlog::trace("libcurl GET {}", url);
    std::string output;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    curl_easy_setopt(curl, CURLOPT_SHARE, shared_handle);
    if (output_needed) {
        output.reserve(1024);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &output);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, string_header_callback);
    } else {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, omit_data_callback);
    }
    set_optional_verbose(curl);
    if (CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
        spdlog::error(
            "curl_easy_perform() failed: {}",
            curl_easy_strerror(res));
        return result<std::string, size_t>(std::string(), -1);
    }
    output.shrink_to_fit();
    return output;
}

result<std::string, size_t> network::request_data(
    bool output_needed,
    std::string_view host,
    std::string_view data)
{
    std::unique_ptr<CURL, curl_free_callback> curl_handle{
        curl_easy_init(),
        curl_easy_cleanup};
    CURL* curl = curl_handle.get();
    if (!curl) {
        spdlog::error("curl_easy_init() failed");
        return result<std::string, size_t>(std::string(), -1);
    }

    spdlog::trace("libcurl GET DATA {} {}", host, data);
    std::string output;
    curl_easy_setopt(curl, CURLOPT_URL, host.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.data());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    if (output_needed) {
        output.reserve(1024);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &output);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, string_header_callback);
    } else {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, omit_data_callback);
    }
    set_optional_verbose(curl);
    if (CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
        spdlog::error(
            "curl_easy_perform() failed: {}",
            curl_easy_strerror(res));
        return result<std::string, size_t>(std::string(), -1);
    }
    output.shrink_to_fit();
    return output;
}

static size_t curl_download_impl(
    void* buffer,
    std::string_view server,
    curl_write_callback write_cb,
    curl_write_callback header_cb =
        nullptr) noexcept(noexcept(write_cb) && noexcept(header_cb))
{
    std::unique_ptr<CURL, curl_free_callback> curl_handle{
        curl_easy_init(),
        curl_easy_cleanup};
    CURL* curl = curl_handle.get();
    if (!curl) {
        spdlog::error("curl_easy_init() failed");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, server.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    if (header_cb) {
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, buffer);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);
    }
    set_optional_verbose(curl);
    if (CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
        spdlog::error(
            "curl_easy_perform() failed: {}",
            curl_easy_strerror(res));
        return -1;
    }
    return 0;
}

size_t network::download(std::string_view filename, std::string_view server)
{
    spdlog::trace("libcurl download, src: {}, dest: {}", server, filename);
    FILE* fp = fopen(filename.data(), "wb");
    if (!fp) {
        spdlog::error("libcurl: failed to open file {}", filename);
        return -1;
    }
    size_t res = curl_download_impl(fp, server, file_callback);
    fclose(fp);
    return res;
}

size_t network::download(std::vector<uint8_t>& buffer, std::string_view server)
{
    spdlog::trace("libcurl download to buffer, src: {}", server);
    buffer.clear();

    size_t res = curl_download_impl(
        &buffer,
        server,
        buffer_callback,
        buffer_header_callback);

    buffer.shrink_to_fit();
    return res;
}

result<std::string, size_t> curl_upload_impl(
    bool output_needed,
    struct curl_httppost* formpost,
    std::string_view server) noexcept(noexcept(string_callback))
{
    std::unique_ptr<CURL, curl_free_callback> curl_handle{
        curl_easy_init(),
        curl_easy_cleanup};
    CURL* curl = curl_handle.get();
    if (!curl) {
        spdlog::error("curl_easy_init() failed");
        curl_formfree(formpost);
        return result<std::string, size_t>(std::string(), -1);
    }
    std::string output;
    curl_easy_setopt(curl, CURLOPT_URL, server.data());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (output_needed) {
        output.reserve(1024);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &output);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, string_header_callback);
    } else {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, omit_data_callback);
    }
    set_optional_verbose(curl);
    if (CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
        spdlog::error(
            "curl_easy_perform() failed: {}",
            curl_easy_strerror(res));
        curl_formfree(formpost);
        return result<std::string, size_t>(std::string(), -1);
    }
    curl_formfree(formpost);
    output.shrink_to_fit();
    return output;
}

result<std::string, size_t> network::upload(
    bool output_needed,
    std::string_view field_name,
    std::string_view filename,
    std::string_view server,
    std::string_view content_type)
{
    spdlog::trace("libcurl upload Content-Type: {}", content_type);
    spdlog::trace("libcurl upload field: {}", field_name);
    spdlog::trace("libcurl upload, src: {}, dest: {}", filename, server);
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    // clang-format off
    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME,    field_name.data(),
        CURLFORM_FILENAME,    filename.data(),
        CURLFORM_FILE,        filename.data(),
        CURLFORM_CONTENTTYPE, content_type.data(),
        CURLFORM_END);
    // clang-format on
    return curl_upload_impl(output_needed, formpost, server);
}

result<std::string, size_t> network::upload(
    bool output_needed,
    const std::vector<uint8_t>& buffer,
    std::string_view field_name,
    std::string_view server,
    std::string_view content_type)
{
    spdlog::trace("libcurl upload Content-Type: {}", content_type);
    spdlog::trace("libcurl upload field: {}", field_name);
    spdlog::trace(
        "libcurl upload buffer: size: {}, dest: {}",
        buffer.size(),
        server);
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    // clang-format off
    curl_formadd(&formpost, &lastptr,
        CURLFORM_BUFFER,         "temp",
        CURLFORM_PTRNAME,        "ptr",
        CURLFORM_COPYNAME,       field_name,
        CURLFORM_BUFFERPTR,      buffer.data(),
        CURLFORM_BUFFERLENGTH,   buffer.size(),
        CURLFORM_CONTENTSLENGTH, buffer.size(),
        CURLFORM_CONTENTTYPE,    content_type.data(),
        CURLFORM_END);
    // clang-format on
    auto result = curl_upload_impl(output_needed, formpost, server);
    if (!result.error()) {
        spdlog::trace(
            "libcurl successfully uploaded {} bytes to {}",
            buffer.size(),
            server);
    }
    return result;
}

}// namespace runtime
