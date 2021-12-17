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

static std::string call(std::string_view                     method,
                        std::map<std::string, std::string>&& params,
                        enum runtime::network::data_flow     output_needed)
{
    namespace net = runtime::network;
    auto [data, error] = net::request(append_url(method), params, output_needed);

    if (error) {
        throw vk::error::runtime_error(-1, "Failed to execute HTTP GET");
    }
    return data;
}

namespace vk::method::policy {

struct group_api
{
    static std::string execute(std::map<std::string, std::string>&& params,
                               const std::string&                   method,
                               const std::string&                   access_token,
                               const std::string&                   user_token,
                               enum runtime::network::data_flow     output_needed)
    {
        VK_UNUSED(user_token);
        params.insert({{"access_token", access_token}, {"v", api_constants::api_version}});
        return call(method, std::move(params), output_needed);
    }
};

struct user_api
{
    static std::string execute(std::map<std::string, std::string>&& params,
                               const std::string&                   method,
                               const std::string&                   access_token,
                               const std::string&                   user_token,
                               enum runtime::network::data_flow     output_needed)
    {
        VK_UNUSED(access_token);
        params.insert({{"access_token", user_token}, {"v", api_constants::api_version}});
        return call(method, std::move(params), output_needed);
    }
};

struct do_not_use_api_link
{
    static std::string execute(std::map<std::string, std::string>&& params,
                               const std::string&                   method,
                               const std::string&                   access_token,
                               const std::string&                   user_token,
                               enum runtime::network::data_flow     output_needed)
    {
        VK_UNUSED(user_token);
        VK_UNUSED(access_token);

        namespace net      = runtime::network;
        auto [data, error] = net::request(method, params, output_needed);

        if (error) {
            throw vk::error::runtime_error(-1, "Failed to execute HTTP GET");
        }
        return data;
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
        std::move(params_),
        method_,
        access_token_,
        user_token_,
        runtime::network::data_flow::require
    );
}

template <typename ExecutionPolicy>
void constructor<ExecutionPolicy>::request_without_output()
{
    ExecutionPolicy::execute(
        std::move(params_),
        method_,
        access_token_,
        user_token_,
        runtime::network::data_flow::omit
    );
}

template class constructor<policy::user_api>;
template class constructor<policy::group_api>;
template class constructor<policy::do_not_use_api_link>;

}// namespace vk::method
