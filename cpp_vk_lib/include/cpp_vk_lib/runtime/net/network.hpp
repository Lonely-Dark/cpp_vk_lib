#ifndef RUNTIME_NET_NETWORK_HPP
#define RUNTIME_NET_NETWORK_HPP

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
 * \throw std::runtime on cURL handle allocation failure or on error during request.
 * \return Retrieved data if output_needed was set to `require`, empty string otherwise.
 */
std::string request(std::string_view host, const std::map<std::string, std::string>& target, data_flow output_needed);
/*!
 * Execute request with POST data.
 *
 * \param host Target URL (e.g https://example.com/).
 * \param data POST data (e.g JSON).
 * \param output_needed Specifies, write cURL output to string or not.
 * \throw std::runtime on cURL handle allocation failure or on error during request.
 * \return Retrieved data if output_needed was set to `require`, empty string otherwise.
 */
std::string request_data(std::string_view host, std::string_view data, data_flow output_needed);
/*!
 * Upload file to server.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param field Upload POST field.
 * \param content_type Content-Type allowed by HTTP.
 * \param filename Path to source file.
 * \param output_needed Specifies, write cURL output to string or not.
 * \throw std::runtime on cURL handle allocation failure or on error during request.
 * \return Retrieved data if output_needed was set to `require`, empty string otherwise.
 */
std::string upload(std::string_view url,
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
 * \throw std::runtime on cURL handle allocation failure or on error during request.
 * \return Retrieved data if output_needed was set to `require`, empty string otherwise.
 */
std::string upload(std::string_view            url,
                   std::string_view            field,
                   std::string_view            content_type,
                   const std::vector<uint8_t>& buffer,
                   data_flow                   output_needed);
/*!
 * Download URL contents to file.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param filename Source filename.
 * \throw std::runtime error on cURL handle allocation failure, or on error during request,
 *        or if file wasn't successfully opened.
 */
void download(std::string_view url, std::string_view filename);
/*!
 * Download URL contents to in-memory buffer.
 *
 * \param url Target URL (e.g https://example.com/).
 * \param buffer Source buffer.
 * \throw std::runtime error on cURL handle allocation failure, or on error during request.
 */
void download(std::string_view url, std::vector<uint8_t>& buffer);

}// namespace runtime::network

#endif// RUNTIME_NET_NETWORK_HPP
