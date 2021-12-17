#include "cpp_vk_lib/vk/error/translate_error.hpp"

#include <unordered_map>

namespace vk::error {

namespace {

enum class error_type : size_t
{
    upload_error,
    access_error,
    invalid_parameter_error,
    runtime_error,
};

struct error
{
    const char* message;
    error_type type;
};

}// namespace

static const std::unordered_map<size_t, error> errors = {
    {1,    {"Unknown error occurred", error_type::runtime_error}},
    {2,    {"Application is disabled. Enable your application or use test mode", error_type::runtime_error}},
    {3,    {"Unknown method passed", error_type::runtime_error}},
    {4,    {"Incorrect signature", error_type::invalid_parameter_error}},
    {5,    {"User authorization failed", error_type::access_error}},
    {6,    {"Too many requests per second", error_type::runtime_error}},
    {7,    {"Permission to perform this action is denied", error_type::access_error}},
    {8,    {"Invalid request", error_type::invalid_parameter_error}},
    {9,    {"Flood control", error_type::runtime_error}},
    {10,   {"Internal server error", error_type::runtime_error}},
    {11,   {"In test mode application should be disabled or user should be authorized", error_type::access_error}},
    {14,   {"Captcha needed", error_type::runtime_error}},
    {15,   {"Access denied", error_type::access_error}},
    {16,   {"HTTP authorization failed", error_type::runtime_error}},
    {17,   {"Validation required", error_type::access_error}},
    {18,   {"User was deleted or banned", error_type::access_error}},
    {20,   {"Permission to perform this action is denied for non-standalone applications", error_type::access_error}},
    {21,   {"Permission to perform this action is allowed only for Standalone and OpenAPI applications", error_type::access_error}},
    {23,   {"This method was disabled", error_type::access_error}},
    {24,   {"Confirmation required", error_type::access_error}},
    {27,   {"Group authorization failed", error_type::access_error}},
    {28,   {"Application authorization failed", error_type::access_error}},
    {29,   {"Rate limit reached", error_type::runtime_error}},
    {30,   {"This profile is private", error_type::access_error}},
    {33,   {"Not implemented yet", error_type::access_error}},
    {37,   {"User was banned", error_type::access_error}},
    {38,   {"Unknown application", error_type::runtime_error}},
    {39,   {"Unknown user", error_type::runtime_error}},
    {40,   {"Unknown group", error_type::runtime_error}},
    {41,   {"Additional signup required", error_type::runtime_error}},
    {100,  {"One of the parameters specified was missing or invalid", error_type::invalid_parameter_error}},
    {101,  {"Invalid application API ID", error_type::invalid_parameter_error}},
    {103,  {"Out of limits", error_type::runtime_error}},
    {113,  {"Invalid user id", error_type::invalid_parameter_error}},
    {150,  {"Invalid timestamp", error_type::invalid_parameter_error}},
    {200,  {"Access to album denied", error_type::access_error}},
    {201,  {"Access to audio denied", error_type::access_error}},
    {203,  {"Access to group denied", error_type::access_error}},
    {300,  {"This album is full", error_type::runtime_error}},
    {500,  {"Permission denied. You must enable votes processing in application settings", error_type::access_error}},
    {600,  {"Permission denied. You have no access to operations specified with given object(s)", error_type::access_error}},
    {925,  {"You are not admin of this chat", error_type::access_error}},
    {932,  {"Your community can't interact with this peer", error_type::access_error}},
    {936,  {"Contact not found", error_type::runtime_error}},
    {939,  {"Message request already sent", error_type::runtime_error}},
    {945,  {"Chat was disabled", error_type::access_error}},
    {946,  {"Chat not supported", error_type::runtime_error}},
    {947,  {"Can't add user to chat, because user has no access to group", error_type::access_error}},
    {603,  {"Some ads error occured", error_type::runtime_error}},
    {3300, {"Recaptcha needed", error_type::access_error}},
    {3301, {"Phone validation needed", error_type::access_error}},
    {3302, {"Password validation needed", error_type::access_error}},
    {3303, {"Otp app validation needed", error_type::access_error}},
    {3304, {"Email confirmation needed", error_type::access_error}},
    {3305, {"Assert votes", error_type::access_error}},
    {3609, {"Token extension required", error_type::access_error}},
    {3610, {"User is deactivated", error_type::access_error}},
    {3611, {"Service is deactivated for user", error_type::access_error}}};

}// namespace vk::error

namespace vk {

const char* error::translate_to_string(size_t error_code)
{
    return errors.at(error_code).message;
}

}// namespace vk
