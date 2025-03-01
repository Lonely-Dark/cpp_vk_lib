#include "cpp_vk_lib/runtime/misc/cppdefs.hpp"

static size_t curl_omit_cb(char* contents, size_t size, size_t nmemb, void* stream) noexcept
{
    VK_UNUSED(contents);
    VK_UNUSED(stream);
    return size * nmemb;
}

static size_t curl_string_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    static_cast<std::string*>(stream)->append(contents, size * nmemb);
    return size * nmemb;
}

static size_t curl_buffer_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    if (!stream) {
        return size * nmemb;
    }
    auto vector = static_cast<std::vector<uint8_t>*>(stream);
    std::copy(contents, contents + (size * nmemb), std::back_inserter(*vector));
    return size * nmemb;
}

static size_t curl_file_cb(char* contents, size_t size, size_t nmemb, void* stream) noexcept
{
    return fwrite(contents, size, nmemb, static_cast<FILE*>(stream));
}

static size_t curl_string_header_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    if (!stream) {
        return size * nmemb;
    }
    size_t bytes = 0U;
    sscanf(contents, "content-length: %zu\n", &bytes);
    if (bytes > 0U) {
        static_cast<std::string*>(stream)->reserve(bytes);
    }
    return size * nmemb;
}

static size_t curl_buffer_header_cb(char* contents, size_t size, size_t nmemb, void* stream)
{
    if (!stream) {
        return size * nmemb;
    }
    size_t bytes = 0U;
    sscanf(contents, "content-length: %zu\n", &bytes);
    if (bytes > 0) {
        static_cast<std::vector<uint8_t>*>(stream)->reserve(bytes);
    }
    return size * nmemb;
}
