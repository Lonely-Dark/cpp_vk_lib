#ifndef VK_METHODS_MESSAGE_CONSTRUCTOR_HPP
#define VK_METHODS_MESSAGE_CONSTRUCTOR_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"
#include "cpp_vk_lib/vk/methods/mentions.hpp"

#include <vector>

namespace vk::method {
/*! constructor<policy::group_api> adapter. */
class message_constructor
{
public:
    /*!
     * Create constructor with following parameters:
     * - random_id=0
     * - disable_mentions=0 || 1, depends on flag
     */
    explicit message_constructor(enum mentions flag);
    /*! constructor<policy::group_api> proxy. */
    message_constructor& param(std::string_view key, std::string_view value);
    /*! Serialize and append attachment list to form `attachments=(serialized output)` */
    message_constructor& attachments(const std::vector<attachment::attachment_ptr_t>&);
    /*! Serialize and append attachment list to form `attachments=(serialized output)` */
    message_constructor& attachments(std::vector<attachment::attachment_ptr_t>&&);
    /*! constructor<policy::group_api> proxy. */
    std::string perform_request();
    /*! constructor<policy::group_api> proxy. */
    void request_without_output();

private:
    group_constructor constructor_;
};

}// namespace vk::method

#endif// VK_METHODS_MESSAGE_CONSTRUCTOR_HPP
