#ifndef VK_METHODS_BASIC_HPP
#define VK_METHODS_BASIC_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "cpp_vk_lib/vk/error/error_code.hpp"

#include <vector>

namespace simdjson::dom {
class object;
}// namespace simdjson::dom

namespace vk::method::messages {

void send(int64_t peer_id, std::string_view text, bool mentions_flag = false);
void send(
    int64_t peer_id,
    std::string_view text,
    std::vector<attachment::attachment_ptr_t>&& list,
    bool mentions_flag = false);
void send(
    int64_t peer_id,
    std::string_view text,
    std::string_view keyboard_layout,
    bool mentions_flag = false);

}// namespace vk::method::messages

namespace vk::method::groups {

int64_t get_by_id(error_code& errc);
std::string get_long_poll_server(int64_t group_id);

}// namespace vk::method::groups

#endif// VK_METHODS_BASIC_HPP
