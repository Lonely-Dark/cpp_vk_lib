#include "cpp_vk_lib/runtime/net/network.hpp"

#include "callback.hpp"
#include "cpp_vk_lib/runtime/uncopyable.hpp"
#include "cpp_vk_lib/runtime/unmovable.hpp"

#include <curl/curl.h>
#include <iostream>
#include <map>
#include <thread>

bool cpp_vk_lib_curl_verbose;

namespace impl {

class libcurl_wrap : public runtime::uncopyable, public runtime::unmovable
{
public:
    libcurl_wrap()
    {
        setup_shared_handle();
    }

    ~libcurl_wrap()
    {
        for (const auto& [thread_id, handle] : handles_) {
            if (handle) {
                curl_easy_cleanup(handle);
            }
        }
        curl_share_cleanup(shared_handle_);
    }

    static libcurl_wrap* get()
    {
        if (!wrap_) {
            wrap_ = new libcurl_wrap;
        }

        return wrap_;
    }

    static CURL* create(std::string_view url)
    {
        auto current_thread_handle = [&]() mutable {
            const std::thread::id thread_id = std::this_thread::get_id();
            if (handles_.find(thread_id) == handles_.end()) {
                handles_.insert(std::make_pair(thread_id, curl_easy_init()));
            }
            return handles_[thread_id];
        };

        CURL* handle = current_thread_handle();
        curl_easy_reset(handle);
        if (!handle) {
            spdlog::error("curl_easy_init() failed");
            exit(-1);
        }
        curl_easy_setopt(handle, CURLOPT_URL, url.data());
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 600L);
        curl_easy_setopt(handle, CURLOPT_USERAGENT, "cpp_vk_lib libcurl-agent/1.1");
        curl_easy_setopt(handle, CURLOPT_SHARE, shared_handle_);
        curl_easy_setopt(handle, CURLOPT_VERBOSE, cpp_vk_lib_curl_verbose ? 1L : 0L);
        return handle;
    }

private:
    static void setup_shared_handle()
    {
        for (auto& mutex : share_data_lock_) {
            pthread_mutex_init(&mutex, nullptr);
        }

        shared_handle_ = curl_share_init();
        if (!shared_handle_) {
            spdlog::error("curl_share_init() failed");
            exit(-1);
        }
        curl_share_setopt(shared_handle_, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(shared_handle_, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        curl_share_setopt(shared_handle_, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
        curl_share_setopt(shared_handle_, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
        curl_share_setopt(shared_handle_, CURLSHOPT_LOCKFUNC, libcurl_lock_cb);
        curl_share_setopt(shared_handle_, CURLSHOPT_UNLOCKFUNC, libcurl_unlock_cb);
    }

    static void libcurl_lock_cb(CURL* handle, curl_lock_data data, curl_lock_access access, void* stream) noexcept
    {
        (void)(handle);
        (void)(access);
        (void)(stream);
        pthread_mutex_lock(&share_data_lock_[data]);
    }

    static void libcurl_unlock_cb(CURL* handle, curl_lock_data data, void* stream) noexcept
    {
        (void)(handle);
        (void)(stream);
        pthread_mutex_unlock(&share_data_lock_[data]);
    }

    static std::map<std::thread::id, CURL*> handles_;
    static libcurl_wrap* wrap_;
    static CURLSH* shared_handle_;
    static pthread_mutex_t share_data_lock_[CURL_LOCK_DATA_LAST];
};

std::map<std::thread::id, CURL*> libcurl_wrap::handles_{};
libcurl_wrap* libcurl_wrap::wrap_    = nullptr;
CURLSH* libcurl_wrap::shared_handle_ = nullptr;
pthread_mutex_t libcurl_wrap::share_data_lock_[CURL_LOCK_DATA_LAST];

static libcurl_wrap* curl_handle()
{
    return libcurl_wrap::get();
}

[[maybe_unused]] int at_exit_handler = []() {
    std::atexit([] {
        curl_handle()->~libcurl_wrap();
    });
    return 0;
}();

}// namespace impl

namespace impl {

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
        char* encoded   = curl_easy_escape(nullptr, url.data(), url.length());
        std::string res = encoded;
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

}// namespace impl

namespace impl {

static runtime::result<std::string, bool>
    libcurl_to_string_recv(CURL* handle, enum runtime::network::data_flow output_needed)
{
    std::string output;
    if (output_needed == runtime::network::data_flow::require) {
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, libcurl_string_write_cb);
        curl_easy_setopt(handle, CURLOPT_HEADERDATA, &output);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, libcurl_string_header_cb);
    } else {
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, libcurl_omit_string_cb);
    }
    if (const CURLcode res = curl_easy_perform(handle); res != CURLE_OK) {
        spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
        return {"", true};
    }
    output.shrink_to_fit();
    return {std::move(output), false};
}

static bool libcurl_to_buffer_recv(
    void* buffer, std::string_view server, curl_write_callback write_cb,
    curl_write_callback header_cb = nullptr) noexcept
{
    CURL* handle = impl::curl_handle()->create(server);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, buffer);
    if (header_cb) {
        curl_easy_setopt(handle, CURLOPT_HEADERDATA, buffer);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_cb);
    }
    if (const CURLcode res = curl_easy_perform(handle); res != CURLE_OK) {
        spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
        return true;
    }
    return false;
}

static runtime::result<std::string, bool> libcurl_from_buffer_send(
    struct curl_httppost* formpost, std::string_view server, enum runtime::network::data_flow output_needed)
{
    CURL* handle = impl::curl_handle()->create(server);
    curl_easy_setopt(handle, CURLOPT_HTTPPOST, formpost);
    auto result = libcurl_to_string_recv(handle, output_needed);
    curl_formfree(formpost);
    return result;
}

}// namespace impl

namespace runtime::network {

result<std::string, bool> request(const request_context& ctx)
{
    const auto& host   = ctx.host.value();
    const auto& target = ctx.target.value();
    auto output_needed = ctx.output_needed;

    const std::string url = impl::create_url(host, target);
    return impl::libcurl_to_string_recv(impl::curl_handle()->create(url), output_needed);
}

result<std::string, bool> request_data(const request_context& ctx)
{
    const auto& host   = ctx.host.value();
    const auto& data   = ctx.request_data.value();
    auto output_needed = ctx.output_needed;

    CURL* handle = impl::curl_handle()->create(host);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, data.data());
    return impl::libcurl_to_string_recv(handle, output_needed);
}

result<std::string, bool> upload(const request_context& ctx)
{
    const auto& filename_opt = ctx.io_filename;
    const auto& buffer_opt   = ctx.io_buffer_ptr;
    const auto& field_name   = ctx.upload_field.value();
    const auto& content_type = ctx.upload_content_type.value();
    const auto& server       = ctx.io_server.value();
    auto output_needed       = ctx.output_needed;

    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr  = nullptr;
    if (filename_opt) {
        const auto& filename = filename_opt.value();
        // clang-format off
        curl_formadd(&formpost, &lastptr,
            CURLFORM_COPYNAME,     field_name.data(),
            CURLFORM_FILENAME,     filename.data(),
            CURLFORM_FILE,         filename.data(),
            CURLFORM_CONTENTTYPE,  content_type.data(),
            CURLFORM_END);
        // clang-format on
        return impl::libcurl_from_buffer_send(formpost, server, output_needed);
    }
    if (buffer_opt) {
        const auto& buffer = *buffer_opt.value();
        // clang-format off
        curl_formadd(&formpost, &lastptr,
            CURLFORM_BUFFER,          "temp",
            CURLFORM_PTRNAME,         "ptr",
            CURLFORM_COPYNAME,        field_name.data(),
            CURLFORM_BUFFERPTR,       buffer.data(),
            CURLFORM_BUFFERLENGTH,    buffer.size(),
            CURLFORM_CONTENTSLENGTH,  buffer.size(),
            CURLFORM_CONTENTTYPE,     content_type.data(),
            CURLFORM_END);
        // clang-format on
        return impl::libcurl_from_buffer_send(formpost, server, output_needed);
    }
    throw std::runtime_error("Neither filename or buffer was defined");
}

bool download(request_context& ctx)
{
    const auto& filename_opt = ctx.io_filename;
    auto& buffer_opt         = ctx.io_buffer_ptr;
    const auto& server       = ctx.io_server.value();

    if (filename_opt) {
        const auto& filename = filename_opt.value();

        std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(filename.data(), "wb"), fclose);
        if (!fp) {
            spdlog::error("libcurl: failed to open file {}", filename);
            return false;
        }
        return impl::libcurl_to_buffer_recv(fp.get(), server, libcurl_file_write_cb);
    }
    if (buffer_opt) {
        auto& buffer = *buffer_opt.value();

        buffer.clear();
        return impl::libcurl_to_buffer_recv(&buffer, server, libcurl_buffer_write_cb, libcurl_buffer_header_cb);
    }
    throw std::runtime_error("Neither filename or buffer was defined");
}

}// namespace runtime::network