#ifndef VK_METHODS_MENTIONS_HPP
#define VK_METHODS_MENTIONS_HPP

namespace vk::method {
/*! Mentions flag for messages.send method. */
enum struct mentions
{
    enable,
    disable
};

constexpr const char* to_string(enum mentions m) noexcept
{
    switch (m) {
        case mentions::disable: {
            return "mentions::disable";
        }
        case mentions::enable: {
            return "mentions::enable";
        }
        default: {
            return "";
        }
    }
}

}// namespace vk::method

#endif// VK_METHODS_MENTIONS_HPP