/*!
 * \mainpage
 *
 * # Organization
 *   cpp_vk_lib is a double library, which is formed from **vk** and
 *   **runtime** modules, where **vk** depends on **runtime**.
 *
 *   Since most devices today uses CPUs with asynchronous execution support,
 *   cpp_vk_lib is pretty thread-safe. Moreover, event processing
 *   system (long polling) is designed to be asynchronous.
 *
 *   **Runtime** module contains
 *   - \ref runtime::network "the networking module";
 *   - \ref runtime::string_utils "string utilities";
 *   - \ref runtime::setup_logger "logger environment";
 *   - some utility classes
 *       \ref runtime::uncopyable "here" and
 *       \ref runtime::unmovable "here".
 *
 *   **Vk** module contains
 *   - \ref vk::attachment "wrapper for attachments";
 *   - \ref vk::error "own exception hierarchy";
 *   - \ref vk::config "external configuration";
 *   - \ref vk::event "incoming event representations";
 *   - \ref vk::keyboard "keyboard";
 *   - \ref vk::long_poll "long poll API";
 *   - \ref vk::method "methods API";
 *   - \ref vk::oauth "OAuth";
 *
 * # Usage
 *   At the moment I see 2 uses for cpp_vk_lib.
 *
 *   1) To build chat-bots based on group long polling. To find more about
 *      designing bots using this library, you can visit
 *      [this repo](https://github.com/epoll-reactor/template_cpp_vk_bot),
 *      where placed ready-to-use implementation.
 *
 *   2) To build small utilities to work with VK, e.g downloading media,
 *      sending messages, collecting data for analysis and so on. Of course,
 *      it makes sense to consider using another VK wrapper for these
 *      purposes, which is easier to setup and use.
 *
 * # About API methods
 *   Library has a couple of commonly used methods, like a
 *   \ref vk::method::messages::send or
 *   \ref vk::method::groups::get_long_poll_server, but in general
 *   user should implement needed for him functionality.
 *
 *   However, there is an interface to do that. There is a few instruments
 *   used for methods constructions:
 *   - \ref vk::method::group_constructor used for methods which require
 *     a group access token.
 *   - \ref vk::method::user_constructor used for methods which require
 *     a user access token.
 *   - \ref vk::method::raw_constructor used for specifying API method
 *     parameters without adding ones implicitly (explained below).
 *
 *   \ref vk::method::group_constructor "group" and
 *   \ref vk::method::user_constructor "user" method constructors adding
 *   to final request string epilogue (**api.vk.com/method** part of URL)
 *   and the parameter list **access_token** and **v** (VK API version) fields, unlike
 *   \ref vk::method::raw_constructor, that does not adding anything.
 *
 *   ## Examples
 *     \code
 *       vk::user_constructor constructor; // You can optionally pass the user token to constructor.
 *       constructor
 *         .method("utils.resolveScreenName")
 *         .param("screen_name", "durov")
 *         .perform_request(); // Passed above data destroys there.
 *       // Correctly creates and does request to API.
 *     \endcode
 *
 *     <br>
 *
 *     \code
 *       vk::group_constructor()
 *         .method("utils.resolveScreenName")
 *         .param("screen_name", "durov")
 *         .perform_request(); // Passed above data destroys there.
 *       // Correctly creates and does request to API.
 *     \endcode
 *
 *     <br>
 *
 *     \code
 *       vk::raw_constructor;
 *         .method("https://lp.vk.com/wh111111111?") // For example, to illustrate long polling.
 *         .param("act", "a_check")
 *         ...
 *         .perform_request(); // Passed above data destroys there.
 *       // Correctly creates and does request to API.
 *     \endcode
 *
 * # CMake options
 *   **-D CPP_VK_LIB_STATIC_BUILD**
 *   - 0 - build shared library (default)
 *   - 1 - build static library
 *
 *   **-D CPP_VK_LIB_SANITIZE**
 *   - 0 - build library without memory and control flow sanitizers (default)
 *   - 1 - build library with memory and control flow sanitizers
 *
 *   **-D CPP_VK_LIB_BENCHMARKING**
 *   - 0 - build benchmarks (default)
 *   - 1 - do not build benchmarks
 *
 * # External variables
 *   **bool cpp_vk_lib_curl_verbose**
 *   - Enable or disable detailed cURL logging
 *
 * # Plans for the future
 *   - Implement user long poll next to group long poll
 *
 * # Note
 *   Please, check the [README.md](https://github.com/epoll-reactor/cpp_vk_lib).
 */

#ifndef VK_CPP_VK_LIB_HPP
#define VK_CPP_VK_LIB_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/error/ensure_api_request_succeeded.hpp"
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
