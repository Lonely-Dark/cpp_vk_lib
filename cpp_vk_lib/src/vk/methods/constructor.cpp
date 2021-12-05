#include "cpp_vk_lib/vk/methods/constructor.hpp"

#include "cpp_vk_lib/runtime/misc/cppdefs.hpp"
#include "cpp_vk_lib/runtime/net/network.hpp"
#include "cpp_vk_lib/vk/api_constants.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/error/exception.hpp"
#include "spdlog/fmt/fmt.h"

static std::string append_url(std::string_view method)
{
    return fmt::format("https://api.vk.com/method/{}?", method);
}

static std::string call(bool output_needed, std::string_view method, std::map<std::string, std::string>&& params)
{
    runtime::network::request_context ctx;
    ctx.output_needed = output_needed;
    ctx.host = append_url(method);
    ctx.target = std::move(params);
    auto response = runtime::network::request(ctx);

    if (response.error()) {
        throw vk::error::runtime_error(response.error(), "Failed to execute HTTP GET");
    }
    return response.value();
}

namespace vk::method::policy {

struct group_api
{
    static std::string execute(
        bool output_needed, std::map<std::string, std::string>&& params, const std::string& method,
        const std::string& access_token, const std::string& user_token)
    {
        VK_UNUSED(user_token);
        params.insert({{"access_token", access_token}, {"v", api_constants::api_version}});
        return call(output_needed, method, std::move(params));
    }
};

struct user_api
{
    static std::string execute(
        bool output_needed, std::map<std::string, std::string>&& params, const std::string& method,
        const std::string& access_token, const std::string& user_token)
    {
        VK_UNUSED(access_token);
        params.insert({{"access_token", user_token}, {"v", api_constants::api_version}});
        return call(output_needed, method, std::move(params));
    }
};

struct do_not_use_api_link
{
    static std::string execute(
        bool output_needed, std::map<std::string, std::string>&& params, const std::string& method,
        const std::string& access_token, const std::string& user_token)
    {
        VK_UNUSED(user_token);
        VK_UNUSED(access_token);
        runtime::network::request_context ctx;
        ctx.output_needed = output_needed;
        ctx.host = method;
        ctx.target = std::move(params);
        auto response = runtime::network::request(ctx);

        if (response.error()) {
            throw vk::error::runtime_error(response.error(), "Failed to execute HTTP GET");
        }
        return response.value();
    }
};

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
constructor<ExecutionPolicy>& constructor<ExecutionPolicy>::param(std::string_view key, std::string_view value)
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
    ExecutionPolicy::execute(runtime::network::omit_data, std::move(params_), method_, access_token_, user_token_);
}

template class constructor<policy::user_api>;
template class constructor<policy::group_api>;
template class constructor<policy::do_not_use_api_link>;

}// namespace vk::method
