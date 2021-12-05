static size_t libcurl_omit_string_cb(char* contents, size_t size, size_t nmemb, void* stream) noexcept
{
    (void)(contents);
    (void)(stream);
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
