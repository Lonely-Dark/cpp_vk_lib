/*!
 * \mainpage
 *
 * ### CMake options:
 * **-D CPP_VK_LIB_STATIC_BUILD**
 * - 0 - build shared library (default)
 * - 1 - build static library
 *
 * **-D CPP_VK_LIB_SANITIZE**
 * - 0 - build library without memory and control flow sanitizers
 * - 1 - build library with memory and control flow sanitizers
 *
 * **-D CPP_VK_LIB_BENCHMARKING**
 * - 0 - build benchmarks
 * - 1 - do not build benchmarks
 *
 * ### Extern variables:
 * **bool cpp_vk_lib_curl_verbose**
 * - Enable or disable detailed cURL logging
 *
 * Please, check the
 * [README.md](https://github.com/threadpoolexecutor/cpp_vk_lib).
 */

#ifndef VK_CPP_VK_LIB_HPP
#define VK_CPP_VK_LIB_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/error/translate_error.hpp"
#include "cpp_vk_lib/vk/events/action.hpp"
#include "cpp_vk_lib/vk/events/attachment_handler.hpp"
#include "cpp_vk_lib/vk/events/common.hpp"
#include "cpp_vk_lib/vk/events/message_new.hpp"
#include "cpp_vk_lib/vk/events/wall_post_new.hpp"
#include "cpp_vk_lib/vk/events/wall_reply_new.hpp"
#include "cpp_vk_lib/vk/keyboard/colors.hpp"
#include "cpp_vk_lib/vk/keyboard/flags.hpp"
#include "cpp_vk_lib/vk/keyboard/layout.hpp"
#include "cpp_vk_lib/vk/long_poll/long_poll.hpp"
#include "cpp_vk_lib/vk/oauth/client.hpp"

#endif// VK_CPP_VK_LIB_HPP
