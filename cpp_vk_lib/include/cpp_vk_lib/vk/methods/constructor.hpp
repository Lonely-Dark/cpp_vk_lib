#ifndef VK_METHODS_CONSTRUCTOR_HPP
#define VK_METHODS_CONSTRUCTOR_HPP

#include "cpp_vk_lib/runtime/net/network.hpp"

namespace vk::method::policy {
/*! Policy that uses group_token. */
struct group_api;
/*! Policy that uses user_token. */
struct user_api;
/*! Raw policy that ignores both tokens. */
struct do_not_use_api_link;

}// namespace vk::method::policy

namespace vk::method {
/*!
 * Basic class to construct VK API requests.
 * Tokens and API version appends automatically.
 *
 * \note All passed parameters and method name are
 * destroyed after performing request, but tokens passed
 * via constructors remains. After request, object is
 * ready to perform next one.
 * \code
 * constructor<user_api> constructor("user_token"); // Remains after request.
 * constructor
 *   .method("utils.resolveScreenName")
 *   .param("screen_name", "durov")
 *   .perform_request(); // Passed above data destroys there.
 * // Correctly creates and does request to API.
 * constructor
 *   .method("messages.send")
 *   .param("text", "Hello, World")
 *   .perform_request(); // Passed above data destroys there.
 * \endcode
 */
template <typename ExecutionPolicy>
class constructor
{
public:
    /*! Load user and group tokens from config. */
    constructor();
    /*! Load group token from config and user token from parameter. */
    constructor(std::string_view user_token);
    /*! Setup VK API method name. */
    constructor& method(std::string_view);
    /*! Append \c param=value pair to parameters list. */
    constructor& param(std::string_view, std::string_view);
    /*! Append map to parameters list. */
    constructor& append_map(std::map<std::string, std::string>&&);
    /*! Execute HTTP POST request and return output. */
    std::string perform_request();
    /*! Execute HTTP POST request without returning anything. */
    void request_without_output();

private:
    void reset();

    std::string user_token_;
    std::string access_token_;
    std::string method_;
    std::map<std::string, std::string> params_;
};

using user_constructor  = constructor<policy::user_api>;
using group_constructor = constructor<policy::group_api>;
using raw_constructor   = constructor<policy::do_not_use_api_link>;

}// namespace vk::method

#endif// VK_METHODS_CONSTRUCTOR_HPP
