#ifndef VK_OAUTH_CLIENT_HPP
#define VK_OAUTH_CLIENT_HPP

#include <memory>
#include <string>

namespace simdjson::dom {
class parser;
}// namespace simdjson::dom

namespace vk::oauth {

enum class target_client : uint8_t
{
    android = (1 << 0),
    iphone = (1 << 1),
    windows = (1 << 2)
};

/*!
 * \brief VK Oauth client.
 *
 *  Example usage:
 *
 *  \code
      int main() {
        vk::oauth::client client("phone number", "password",
          vk::oauth::target_client::windows);
        client.pull();
        vk::method::constructor(client.token())
          .method(...)
          .param(...)
          .perform_request();
      }
 *  \endcode
 */
class client
{
public:
    client(std::string_view username, std::string_view password, target_client client_type);
    /*!
     * Try get user data.
     *
     * \throw vk::exception::access_error with detailed description in case,
     *        when wrong data were provided.
     */
    void pull();
    const std::string& token() const noexcept;
    int64_t user_id() const noexcept;

private:
    target_client client_type_;
    std::string username_;
    std::string password_;

    std::string target_client_secret_;
    int64_t target_client_id_;

    std::string pulled_token_;
    int64_t pulled_user_id_;
};

}// namespace vk::oauth

#endif// VK_OAUTH_CLIENT_HPP
