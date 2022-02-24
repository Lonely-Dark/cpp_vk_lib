#include "cpp_vk_lib/vk/oauth/client.hpp"

#include "cpp_vk_lib/vk/api_constants.hpp"
#include "cpp_vk_lib/vk/error/exception.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

namespace vk::oauth {

client::client(std::string_view username, std::string_view password, vk::oauth::target_client client_type)
    : client_type_(client_type)
    , username_(username)
    , password_(password)
    , target_client_secret_()
    , target_client_id_()
    , pulled_token_()
    , pulled_user_id_(0)
{
    switch (client_type_) {
        case target_client::android: {
            target_client_id_     = api_constants::android_app_client_id;
            target_client_secret_ = api_constants::android_app_client_secret;
            break;
        }
        case target_client::iphone: {
            target_client_id_     = api_constants::iphone_app_client_id;
            target_client_secret_ = api_constants::iphone_app_client_secret;
            break;
        }
        case target_client::windows: {
            target_client_id_     = api_constants::windows_app_client_id;
            target_client_secret_ = api_constants::windows_app_client_secret;
            break;
        }
    }
    pull();
}

static bool error_returned(const simdjson::dom::object& response, std::string_view error_desc)
{
    return response.begin().key() == "error" && response["error"].get_string().take_value() == error_desc;
}

void client::pull()
{
    method::raw_constructor constructor;
    constructor.method(std::string(api_constants::oauth_link) + "token?")
        .param("grant_type", "password")
        .param("client_id", std::to_string(target_client_id_))
        .param("client_secret", target_client_secret_)
        .param("username", username_.data())
        .param("password", password_.data());

    simdjson::dom::parser parser;
    const simdjson::dom::object response = parser.parse(constructor.perform_request());

    if (error_returned(response, "invalid_client") ||
        error_returned(response, "invalid_request") ||
        error_returned(response, "invalid_grant")) {
        throw error::access_error(-1, response["error_description"].get_c_str().take_value());
    }

    pulled_token_ = response["access_token"].get_c_str().take_value();
    pulled_user_id_ = response["user_id"].get_int64();

    spdlog::debug("OAuth token: {}", pulled_token_);
    spdlog::debug("OAuth user_id: {}", pulled_user_id_);
}

const std::string& client::token() const noexcept
{
    return pulled_token_;
}

int64_t client::user_id() const noexcept
{
    return pulled_user_id_;
}

}// namespace vk::oauth
