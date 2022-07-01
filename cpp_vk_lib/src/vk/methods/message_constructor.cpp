#include "cpp_vk_lib/vk/methods/message_constructor.hpp"

template <typename Attachment>
static std::string serialize_attachment(Attachment&& attachment)
{
    spdlog::trace("1 attachment successfully serialized");

    return attachment->value();
}

template <typename AttachmentList>
static std::string serialize_attachments(AttachmentList&& attachments)
{
    std::string result;
    result.reserve(attachments.size() * 20 /* Average attachment string-value length. */);

    for (const auto& attachment : attachments) {
        result += attachment->value();
        result += ',';
    }

    spdlog::trace("{} attachment(s) successfully serialized", attachments.size());

    return result;
}

namespace vk::method {

message_constructor::message_constructor(enum mentions flag)
{
    constructor_.method("messages.send");

    param("random_id", "0");
    param("disable_mentions", (flag == mentions::disable) ? "1" : "0");
}

message_constructor& message_constructor::param(std::string_view lhs, std::string_view rhs)
{
    constructor_.param(lhs, rhs);
    return *this;
}

message_constructor& message_constructor::attachment(const attachment::attachment_ptr_t& attachment)
{
    param("attachment", serialize_attachment(attachment).data());
    return *this;
}

message_constructor& message_constructor::attachments(const std::vector<vk::attachment::attachment_ptr_t>& attachments)
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
