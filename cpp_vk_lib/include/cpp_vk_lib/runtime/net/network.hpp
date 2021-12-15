#ifndef RUNTIME_NET_NETWORK_HPP
#define RUNTIME_NET_NETWORK_HPP

#include "cpp_vk_lib/runtime/result.hpp"

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

/*! Used for handling verbose cURL output. */
extern bool cpp_vk_lib_curl_verbose;

namespace runtime::network {

static constexpr bool require_data = true;
static constexpr bool omit_data    = false;

/*! Wrapper aimed to reduce the great amount of input parameters. */
struct request_context
{
    std::optional<std::map<std::string, std::string>> target{};
    std::optional<std::string> host{};
    std::optional<std::string> request_data{};
    std::optional<std::string> io_filename{};
    std::optional<std::vector<uint8_t>*> io_buffer_ptr{};
    std::optional<std::string> io_server{};
    std::optional<std::string> upload_field{};
    std::optional<std::string> upload_content_type{};
    bool output_needed{false};
};
/*!
 * Perform HTTP POST request.
 * \param ctx Request context.
 * \throw std::bad_optional_access if `host` and `target` fields were not specified.
 *
 * \return Result with payload string and 0 error code on success, empty string and -1 error code
 * otherwise.
 */
result<std::string, size_t> request(const request_context& ctx);
/*!
 * Perform HTTP POST request with specified data.
 * \param ctx Request context.
 * \throw std::bad_optional_access if `host` and `request_data` fields were not specified.
 *
 * \return Result with payload string and 0 error code on success, empty string and -1 error code
 * otherwise.
 */
result<std::string, size_t> request_data(const request_context& ctx);
/*!
 * Upload bytes from file or buffer to remote server.
 * \param ctx Request context.
 * \throw std::runtime_error if neither filename or buffer was specified.
 * \throw std::bad_optional_access if
 *   `io_filename` or `io_buffer_ptr`,
 *   `upload_field`,
 *   `upload_content_type`,
 *   `io_server` fields were not specified.
 *
 * \return Result with payload string and 0 error code on success, empty string and -1 error code
 * otherwise.
 */
result<std::string, size_t> upload(const request_context& ctx);
/*!
 * Download bytes from remote server to file or buffer.
 * \param ctx Request context.
 * \throw std::runtime_error if neither filename or buffer was specified.
 * \throw std::bad_optional_access if
 *   `io_filename` or `io_buffer_ptr`,
 *   `io_server` fields were not specified.
 * \
 * \return 0 on success, -1 otherwise. (Actually to rework...)
 */
size_t download(request_context& ctx);

}// namespace runtime::network

#endif// RUNTIME_NET_NETWORK_HPP
