#ifndef VK_METHODS_CONSTRUCTOR_HPP
#define VK_METHODS_CONSTRUCTOR_HPP

#include "cpp_vk_lib/runtime/net/network.hpp"

namespace vk::method::policy {
/*! Policy that uses group_token. */
struct group_api;
/*! Policy that uses user_token. */
struct user_api;
/*! Raw policy. All tokens are ignored. */
struct do_not_use_api_link;

}// namespace vk::method::policy

namespace vk::method {
/*!
 * Basic class to construct VK API requests.
 * Tokens and API version appends automatically.
 *
 * \code
     constructor<user_api>()
       .method("utils.resolveScreenName")
       .param("screen_name", "durov")
       .perform_request();
 * \endcode
 */
template <typename ExecutionPolicy>
class constructor
{
public:
    /*! Load user token from config. */
    constructor();
    /*! Initialize user token from parameter. */
    constructor(std::string_view user_token);
    /*! Setup VK API method name. */
    constructor& method(std::string_view);
    /*! Append \c param=value pair to parameters list. */
    constructor& param(std::string_view, std::string_view);
    /*! Append map to parameters list. */
    constructor& append_map(std::map<std::string, std::string>&&);
    /*!
     * Execute HTTP POST request and return output.
     *
     * \note all parameters are destroyed after request.
     */
    std::string perform_request();
    /*!
     * Execute HTTP POST request without returning anything.
     *
     * \note all parameters are destroyed after request.
     */
    void request_without_output();

private:
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
