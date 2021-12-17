#include "cpp_vk_lib/runtime/net/network.hpp"

#include "callback.hpp"
#include "cpp_vk_lib/runtime/uncopyable.hpp"
#include "cpp_vk_lib/runtime/unmovable.hpp"

#include <curl/curl.h>
#include <iostream>
#include <map>
#include <thread>

bool cpp_vk_lib_curl_verbose;

class curl_wrap : public runtime::uncopyable, public runtime::unmovable
{
public:
    curl_wrap()
    {
        setup_shared_handle();
    }

    ~curl_wrap()
    {
        for (const auto& [thread_id, handle] : handles_) {
            if (handle) {
                curl_easy_cleanup(handle);
            }
        }
        curl_share_cleanup(shared_handle_);
    }

    static curl_wrap* get()
    {
        if (!wrap_) {
            wrap_ = new curl_wrap;
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

    static inline curl_wrap* wrap_       = nullptr;
    static inline CURLSH* shared_handle_ = nullptr;
    static inline std::map<std::thread::id, CURL*> handles_;
    static inline pthread_mutex_t share_data_lock_[CURL_LOCK_DATA_LAST];
};

[[maybe_unused]] int atexit_handler = [] {
    std::atexit([] {
        curl_wrap::get()->~curl_wrap();
    });
    return 0;
}();

class curl_executor
{
    using data_flow = runtime::network::data_flow;

public:
    explicit curl_executor(std::string_view url)
        : url_(url)
    {}

    std::pair<std::string, bool> request(const std::map<std::string, std::string>& list, data_flow output_needed)
    {
        CURL* curl_handle = curl_wrap::create(create_url(url_, list));

        return curl_get(curl_handle, output_needed);
    }

    std::pair<std::string, bool> request_data(std::string_view data, data_flow output_needed)
    {
        CURL* curl_handle = curl_wrap::create(url_);

        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data.data());
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, data.size());

        return curl_get(curl_handle, output_needed);
    }

    std::pair<std::string, bool> upload(
        std::string_view field, std::string_view content_type, std::string_view filename, data_flow output_needed)
    {
        curl_httppost* form_post     = nullptr;
        curl_httppost* last_form_ptr = nullptr;

        // clang-format off
        curl_formadd(&form_post, &last_form_ptr,
            CURLFORM_COPYNAME,     field.data(),
            CURLFORM_FILENAME,     filename.data(),
            CURLFORM_FILE,         filename.data(),
            CURLFORM_CONTENTTYPE,  content_type.data(),
            CURLFORM_END
        );
        // clang-format on

        auto result = curl_post(form_post, output_needed);
        curl_formfree(form_post);
        return result;
    }

    std::pair<std::string, bool> upload(
        std::string_view field, std::string_view content_type, const std::vector<uint8_t>& buffer,
        data_flow output_needed)
    {
        curl_httppost* form_post     = nullptr;
        curl_httppost* last_form_ptr = nullptr;

        // clang-format off
        curl_formadd(&form_post, &last_form_ptr,
            CURLFORM_BUFFER,          "temp",
            CURLFORM_PTRNAME,         "ptr",
            CURLFORM_COPYNAME,        field.data(),
            CURLFORM_BUFFERPTR,       buffer.data(),
            CURLFORM_BUFFERLENGTH,    buffer.size(),
            CURLFORM_CONTENTSLENGTH,  buffer.size(),
            CURLFORM_CONTENTTYPE,     content_type.data(),
            CURLFORM_END
        );
        // clang-format on

        auto result = curl_post(form_post, output_needed);
        curl_formfree(form_post);
        return result;
    }

    bool download(std::string_view filename)
    {
        std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(filename.data(), "wb"), fclose);
        if (!fp) {
            spdlog::error("libcurl: failed to open file {}", filename);
            return false;
        }

        CURL* curl_handle = curl_wrap::create(url_);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp.get());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_file_cb);

        CURLcode error_code = curl_easy_perform(curl_handle);

        if (error_code != CURLE_OK) {
            spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(error_code));
            return false;
        }

        return true;
    }

    bool download(std::vector<uint8_t>& buffer)
    {
        CURL* curl_handle = curl_wrap::create(url_);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, &buffer);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_buffer_cb);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, curl_buffer_header_cb);

        CURLcode error_code = curl_easy_perform(curl_handle);

        if (error_code != CURLE_OK) {
            spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(error_code));
            return false;
        }

        return true;
    }

private:
    static std::pair<std::string, bool> curl_get(CURL* curl_handle, data_flow output_needed)
    {
        std::string output;

        if (output_needed == data_flow::require) {
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &output);
            curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, &output);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_string_cb);
            curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, curl_string_header_cb);
        } else {
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_omit_cb);
        }

        CURLcode error_code = curl_easy_perform(curl_handle);

        if (error_code != CURLE_OK) {
            spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(error_code));
            return {"", curl_failure};
        }

        return {std::move(output), curl_success};
    }

    std::pair<std::string, bool> curl_post(curl_httppost* form_post, data_flow output_needed)
    {
        CURL* curl_handle = curl_wrap::create(url_);

        curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, form_post);

        return curl_get(curl_handle, output_needed);
    }

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
            char* encoded   = curl_escape(url.data(), url.length());
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

    std::string_view url_;
    static constexpr bool curl_success = false;
    static constexpr bool curl_failure = true;
};

namespace runtime::network {

std::pair<std::string, bool>
    request(std::string_view host, const std::map<std::string, std::string>& target, data_flow output_needed)
{
    curl_executor executor(host);
    return executor.request(target, output_needed);
}

std::pair<std::string, bool> request_data(std::string_view host, std::string_view data, data_flow output_needed)
{
    curl_executor executor(host);
    return executor.request_data(data, output_needed);
}

std::pair<std::string, bool> upload(
    std::string_view url, std::string_view field, std::string_view content_type, std::string_view filename,
    data_flow output_needed)
{
    curl_executor executor(url);
    return executor.upload(field, content_type, filename, output_needed);
}

std::pair<std::string, bool> upload(
    std::string_view url, std::string_view field, std::string_view content_type, const std::vector<uint8_t>& buffer,
    data_flow output_needed)
{
    curl_executor executor(url);
    return executor.upload(field, content_type, buffer, output_needed);
}

bool download(std::string_view url, std::string_view filename)
{
    curl_executor executor(url);
    return executor.download(filename);
}

bool download(std::string_view url, std::vector<uint8_t>& buffer)
{
    curl_executor executor(url);
    return executor.download(buffer);
}

}// namespace runtime::network
