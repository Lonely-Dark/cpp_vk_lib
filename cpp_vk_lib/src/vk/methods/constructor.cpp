#include "cpp_vk_lib/vk/methods/constructor.hpp"

#include "cpp_vk_lib/runtime/net/network.hpp"
#include "cpp_vk_lib/runtime/string_utils/string_utils.hpp"
#include "cpp_vk_lib/vk/api_constants.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/error/exception.hpp"

static std::string append_url(std::string_view method)
{
    return runtime::string_utils::format("https://api.vk.com/method/{}?", method);
}

static std::string
    call(bool output_needed, std::string_view method, std::map<std::string, std::string>&& params)
{
    auto response(runtime::network::request(output_needed, append_url(method), std::move(params)));
    if (response.error()) {
        throw vk::error::runtime_error(response.error(), "Failed to execute HTTP GET");
    }
    return response.value();
}

namespace vk::method::policy {

struct group_api
{
    static std::string execute(
        bool output_needed,
        std::map<std::string, std::string>&& params,
        const std::string& method,
        const std::string& access_token,
        const std::string& user_token);
};

struct user_api
{
    static std::string execute(
        bool output_needed,
        std::map<std::string, std::string>&& params,
        const std::string& method,
        const std::string& access_token,
        const std::string& user_token);
};

struct do_not_use_api_link
{
    static std::string execute(
        bool output_needed,
        std::map<std::string, std::string>&& params,
        const std::string& method,
        const std::string& access_token,
        const std::string& user_token);
};

std::string group_api::execute(
    bool output_needed,
    std::map<std::string, std::string>&& params,
    const std::string& method,
    const std::string& access_token,
    const std::string& user_token)
{
    (void)user_token;
    params.insert({{"access_token", access_token}, {"v", api_constants::api_version}});
    return call(output_needed, method, std::move(params));
}

std::string user_api::execute(
    bool output_needed,
    std::map<std::string, std::string>&& params,
    const std::string& method,
    const std::string& access_token,
    const std::string& user_token)
{
    (void)access_token;
    params.insert({{"access_token", user_token}, {"v", api_constants::api_version}});
    return call(output_needed, method, std::move(params));
}

std::string do_not_use_api_link::execute(
    bool output_needed,
    std::map<std::string, std::string>&& params,
    const std::string& method,
    const std::string& access_token,
    const std::string& user_token)
{
    (void)user_token;
    (void)access_token;
    auto response(runtime::network::request(output_needed, method, std::move(params)));
    if (response.error()) {
        throw vk::error::runtime_error(response.error(), "Failed to execute HTTP GET");
    }
    return response.value();
}

}// namespace vk::method::policy

namespace vk::method {

template <typename ExecutionPolicy>
constructor<ExecutionPolicy>::constructor()
    : user_token_(config::user_token())
    , access_token_(config::access_token())
{}

template <typename ExecutionPolicy>
constructor<ExecutionPolicy>::constructor(std::string_view user_token)
    : user_token_(user_token)
    , access_token_(config::access_token())
{}

template <typename ExecutionPolicy>
constructor<ExecutionPolicy>& constructor<ExecutionPolicy>::method(std::string_view method)
{
    method_.assign(method.data(), method.size());
    return *this;
}

template <typename ExecutionPolicy>
constructor<ExecutionPolicy>&
    constructor<ExecutionPolicy>::param(std::string_view key, std::string_view value)
{
    params_.emplace(key, value);
    return *this;
}

template <typename ExecutionPolicy>
constructor<ExecutionPolicy>&
    constructor<ExecutionPolicy>::append_map(std::map<std::string, std::string>&& additional_params)
{
    params_.merge(std::move(additional_params));
    return *this;
}

template <typename ExecutionPolicy>
std::string constructor<ExecutionPolicy>::perform_request()
{
    return ExecutionPolicy::execute(
        runtime::network::require_data,
        std::move(params_),
        method_,
        access_token_,
        user_token_);
}

template <typename ExecutionPolicy>
void constructor<ExecutionPolicy>::request_without_output()
{
    ExecutionPolicy::execute(
        runtime::network::omit_data,
        std::move(params_),
        method_,
        access_token_,
        user_token_);
}

}// namespace vk::method

template class vk::method::constructor<vk::method::policy::user_api>;
template class vk::method::constructor<vk::method::policy::group_api>;
template class vk::method::constructor<vk::method::policy::do_not_use_api_link>;
