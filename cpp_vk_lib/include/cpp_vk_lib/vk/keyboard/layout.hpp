#ifndef VK_KEYBOARD_LAYOUT_HPP
#define VK_KEYBOARD_LAYOUT_HPP

#include "cpp_vk_lib/vk/keyboard/buttons/location.hpp"
#include "cpp_vk_lib/vk/keyboard/buttons/open_app.hpp"
#include "cpp_vk_lib/vk/keyboard/buttons/text.hpp"
#include "cpp_vk_lib/vk/keyboard/buttons/vk_pay.hpp"
#include "cpp_vk_lib/vk/keyboard/flags.hpp"

#include <any>
#include <vector>

namespace vk::keyboard {
/*!
 * \brief The buttons grid representation.
 */
class layout
{
public:
    layout() = default;
    layout(keyboard::flag flags);

    void add_row(std::vector<std::any>&&);
    /*!
     * Convert stored buttons data to JSON schema and store output to
     * serialized_.
     */
    void serialize();
    /*!
     * \return reference to serialized layout.
     */
    const std::string& get() const noexcept;
    bool has_flag(keyboard::flag) const noexcept;

private:
    std::string serialized_{};
    std::vector<std::vector<std::any>> buttons_{};
    flag flags_ = vk::keyboard::flag::none;
};

}// namespace vk::keyboard

#endif// VK_KEYBOARD_LAYOUT_HPP
