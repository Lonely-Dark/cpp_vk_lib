#ifndef VK_METHODS_UTILITY_MESSAGE_CONSTRUCTOR_HPP
#define VK_METHODS_UTILITY_MESSAGE_CONSTRUCTOR_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"

#include <vector>

namespace vk::method {
/*!
 * \brief constructor<policy::group_api> adapter.
 */
class message_constructor
{
public:
    static inline bool disable_mentions = true;
    static inline bool enable_mentions  = false;
    /*!
     * Create constructor with following parameters:
     * - random_id=0
     * - disable_mentions=0 or 1, depends on disable_mentions_flag
     */
    explicit message_constructor(bool disable_mentions_flag);
    /*!
     * constructor<policy::group_api> proxy.
     */
    message_constructor& param(std::string_view, std::string_view);
    /*!
     * Serialize and append attachment list to:
     * - attachments=(serialized output)
     */
    message_constructor& attachments(const std::vector<attachment::attachment_ptr_t>&);
    /*!
     * Serialize and append attachment list to:
     * - attachments=(serialized output)
     */
    message_constructor& attachments(std::vector<attachment::attachment_ptr_t>&&);
    /*!
     * constructor<policy::group_api> proxy.
     */
    std::string perform_request();
    /*!
     * constructor<policy::group_api> proxy.
     */
    void request_without_output();

private:
    group_constructor constructor_;
};

}// namespace vk::method

#endif// VK_METHODS_UTILITY_MESSAGE_CONSTRUCTOR_HPP
