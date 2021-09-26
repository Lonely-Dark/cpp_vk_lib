#ifndef VK_EVENTS_HANDLERS_ATTACHMENT_HANDLER_HPP
#define VK_EVENTS_HANDLERS_ATTACHMENT_HANDLER_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"

#include <vector>

namespace simdjson::dom {
class array;
}// namespace simdjson::dom

namespace vk::event {
/*!
 * \brief Extract all attachments from JSON
 *
 * \return Array of valid attachment pointers
 */
std::vector<attachment::attachment_ptr_t> get_attachments(const simdjson::dom::array& attachments);

}// namespace vk::event

#endif// VK_EVENTS_HANDLERS_ATTACHMENT_HANDLER_HPP
