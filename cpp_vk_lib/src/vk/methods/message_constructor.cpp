#include "cpp_vk_lib/vk/methods/message_constructor.hpp"

template <typename AttachmentList>
static std::string serialize_attachments(AttachmentList&& attachments)
{
    std::string result;
    result.reserve(attachments.size() * 20);

    for (auto& attachment : attachments) {
        result += attachment->value();
        result += ',';
    }

    return result;
}

namespace vk::method {

message_constructor::message_constructor(bool disable_mentions_flag)
{
    constructor_.method("messages.send");

    param("random_id", "0");

    if (disable_mentions_flag) {
        param("disable_mentions", "1");
    } else {
        param("disable_mentions", "0");
    }
}

message_constructor& message_constructor::param(std::string_view lhs, std::string_view rhs)
{
    constructor_.param(lhs, rhs);
    return *this;
}

message_constructor&
    message_constructor::attachments(std::vector<vk::attachment::attachment_ptr_t>&& attachments)
{
    param("attachment", serialize_attachments(std::move(attachments)).data());
    return *this;
}

message_constructor& message_constructor::attachments(
    const std::vector<vk::attachment::attachment_ptr_t>& attachments)
{
    param("attachment", serialize_attachments(attachments).data());
    return *this;
}

std::string message_constructor::perform_request()
{
    return constructor_.perform_request();
}

void message_constructor::request_without_output()
{
    constructor_.request_without_output();
}

}// namespace vk::method
