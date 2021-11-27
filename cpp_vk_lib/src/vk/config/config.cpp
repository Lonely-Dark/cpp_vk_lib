#include "cpp_vk_lib/vk/config/config.hpp"

#include "cpp_vk_lib/runtime/misc/cppdefs.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

#include <fstream>

namespace {

class VK_HIDDEN loader
{
public:
    VK_DISABLE_COPY_MOVE(loader)

    static void load(std::string_view path);
    static void load_string(std::string_view string);
    static loader& get();

    std::string& user_token() noexcept;
    const std::string& username() const noexcept;
    const std::string& password() const noexcept;
    const std::string& access_token() const noexcept;
    const std::string& log_path() const noexcept;
    size_t num_workers() const noexcept;

private:
    loader(std::string_view path);

    std::string username_{};
    std::string password_{};
    std::string user_token_{};
    std::string access_token_{};
    std::string log_path_{};
    size_t num_workers_{};

    static loader* instance_;
};

}// anonymous namespace

loader::loader(std::string_view path)
{
    simdjson::dom::parser parser;
    const simdjson::dom::element element = parser.load(path.data());

    username_     = element["oauth"]["login"];
    password_     = element["oauth"]["password"];
    user_token_   = element["api"]["user_token"].get_c_str().take_value();
    access_token_ = element["api"]["access_token"].get_c_str().take_value();
    log_path_     = element["environment"]["log_path"].get_c_str().take_value();
    num_workers_  = element["environment"]["num_workers"].get_int64();

    spdlog::info("config loaded successfully from {}", path);
}

loader* loader::instance_ = nullptr;

void loader::load(std::string_view path)
{
    delete instance_;
    instance_ = new loader(path);
}

void loader::load_string(std::string_view string)
{
    std::ofstream{"tmp_config.json"} << string;
    delete instance_;
    instance_ = new loader("tmp_config.json");
    std::remove("tmp_config.json");
}

loader& loader::get()
{
    if (!instance_) { throw std::runtime_error("Please, load config first."); }

    return *instance_;
}

std::string& loader::user_token() noexcept
{
    return user_token_;
}

const std::string& loader::username() const noexcept
{
    return username_;
}

const std::string& loader::password() const noexcept
{
    return password_;
}

const std::string& loader::access_token() const noexcept
{
    return access_token_;
}

const std::string& loader::log_path() const noexcept
{
    return log_path_;
}

size_t loader::num_workers() const noexcept
{
    return num_workers_;
}

namespace vk {

void config::load(std::string_view path)
{
    loader::load(path);
}

void config::load_string(std::string_view string)
{
    loader::load_string(string);
}

void config::override_user_token(std::string_view token)
{
    loader::get().user_token() = token;
}

const std::string& config::password()
{
    return loader::get().password();
}

const std::string& config::username()
{
    return loader::get().username();
}

const std::string& config::user_token()
{
    return loader::get().user_token();
}

const std::string& config::access_token()
{
    return loader::get().access_token();
}

const std::string& config::log_path()
{
    return loader::get().log_path();
}

size_t config::num_workers()
{
    return loader::get().num_workers();
}

}// namespace vk
