#ifndef VK_METHODS_BASIC_HPP
#define VK_METHODS_BASIC_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "cpp_vk_lib/vk/methods/mentions.hpp"

#include <vector>

namespace vk::method::messages {

/*!
 * Send message.
 *
 * \return raw JSON.
 */
std::string send(int64_t peer_id, std::string_view text, enum mentions mentions = mentions::disable);

/*!
 * Send message.
 *
 * \return raw JSON.
 */
std::string send(int64_t                                          peer_id,
                 std::string_view                                 text,
                 const std::vector<attachment::attachment_ptr_t>& list,
                 enum mentions                                    mentions = mentions::disable);

/*!
 * Send message.
 *
 * \return raw JSON.
 */
std::string send(int64_t                             peer_id,
                 std::string_view                    text,
                 const attachment::attachment_ptr_t& attachment,
                 enum mentions                       mentions = mentions::disable);

/*!
 * Send message.
 *
 * \return raw JSON.
 */
std::string send(int64_t          peer_id,
                 std::string_view text,
                 std::string_view keyboard_layout,
                 enum mentions    mentions = mentions::disable);

}// namespace vk::method::messages

namespace vk::method::groups {

/*!
 * Get ID of group, that owns the token.
 *
 * \return ID of group.
 */
int64_t get_by_id();

/*!
 * Get long poll server.
 *
 * \return raw JSON.
 */
std::string get_long_poll_server(int64_t group_id);

}// namespace vk::method::groups

#endif// VK_METHODS_BASIC_HPP
