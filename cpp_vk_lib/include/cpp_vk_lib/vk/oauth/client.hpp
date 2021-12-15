#ifndef VK_OAUTH_CLIENT_HPP
#define VK_OAUTH_CLIENT_HPP

#include <memory>
#include <string>

namespace simdjson::dom {
class parser;
}// namespace simdjson::dom

namespace vk::oauth {
/*! OAuth targets. */
enum struct target_client
{
    android,
    iphone,
    windows
};

/*!
 * VK Oauth client.
 *
 * \note Please be careful using this class in your open source
 * code. Make sure that you don't store your personal data in
 * string literals of your program. Much better to encapsulate that
 * in an external config file.
 *
 * Example usage:
 *
 *  \code
      int main() {
        vk::oauth::client client(config::phone_number(), config::password(),
          vk::oauth::target_client::windows);
        client.pull();
        vk::method::constructor(client.token())/ * ... * /.perform_request();
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
     * \throws vk::exception::access_error with detailed description in case,
     *         when wrong data were provided.
     */
    void pull();
    const std::string& token() const noexcept;
    int64_t user_id() const noexcept;

private:
    /*! Android, Windows or MacOS. */
    target_client client_type_;
    /*! Email or phone number. */
    std::string username_;
    /*! Account password. */
    std::string password_;

    /*! Hash key associated with target_client. */
    std::string target_client_secret_;
    /*! Client id associated with target_client. */
    int64_t target_client_id_;

    /*! Received from OAuth token. */
    std::string pulled_token_;
    /*! Id of person, who executes pulling. */
    int64_t pulled_user_id_;
};

}// namespace vk::oauth

#endif// VK_OAUTH_CLIENT_HPP
