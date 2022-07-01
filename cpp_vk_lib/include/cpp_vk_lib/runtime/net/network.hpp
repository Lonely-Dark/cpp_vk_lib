#ifndef RUNTIME_NET_NETWORK_HPP
#define RUNTIME_NET_NETWORK_HPP

#include "cpp_vk_lib/runtime/result.hpp"

#include <map>
#include <string>
#include <string_view>
#include <vector>

/*! Used for handling verbose cURL output. */
extern bool cpp_vk_lib_curl_verbose;

namespace runtime::network {

/*! Flag to tell what to do with incoming data. */
enum struct data_flow { require, omit };

/*!
 * Execute request.
 *
 * \param host Target URL (e.g https://example.com/).
 * \param target Parameters list to be serialized to string like value_1=1&value_2=2&value_3=3.
 * \param output_needed Specifies, write cURL output to string or not.
 * \return Pair of string and bool, where bool set to true if an error occurred.
 */
std::pair<std::string, bool> request(std::string_view host, const std::map<std::string, std::string>& target, data_flow output_needed);
/*!
 * Execute request with POST data.
 *
 * \param host Target URL (e.g https://example.com/).
 * \param data POST data (e.g JSON).
 * \param output_needed Specifies, write cURL output to string or not.
 * \return Pair of string and bool, where bool set to true if an error occurred.
 */
std::pair<std::string, bool> request_data(std::string_view host, std::string_view data, data_flow output_needed);
/*!
 * Upload file to server.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param field Upload POST field.
 * \param content_type Content-Type allowed by HTTP.
 * \param filename Path to source file.
 * \param output_needed Specifies, write cURL output to string or not.
 * \return Pair of string and bool, where bool set to true if an error occurred.
 */
std::pair<std::string, bool> upload(std::string_view url,
                                    std::string_view field,
                                    std::string_view content_type,
                                    std::string_view filename,
                                    data_flow        output_needed);
/*!
 * Upload in-memory file representation to server.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param field Upload POST field.
 * \param content_type Content-Type allowed by HTTP.
 * \param buffer Vector of file represented as bytes.
 * \param output_needed Specifies, write cURL output to string or not.
 * \return Pair of string and bool, where bool set to true if an error occurred.
 */
std::pair<std::string, bool> upload(std::string_view            url,
                                    std::string_view            field,
                                    std::string_view            content_type,
                                    const std::vector<uint8_t>& buffer,
                                    data_flow                   output_needed);
/*!
 * Download URL contents to file.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param filename Source filename.
 * \return true on success, false on error.
 */
bool download(std::string_view url, std::string_view filename);
/*!
 * Download URL contents to in-memory buffer.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param buffer Source buffer.
 * \return true on success, false on error.
 */
bool download(std::string_view url, std::vector<uint8_t>& buffer);

}// namespace runtime::network

#endif// RUNTIME_NET_NETWORK_HPP
