#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/runtime/signal_handlers.hpp"
#include "cpp_vk_lib/vk/api_constants.hpp"
#include "cpp_vk_lib/vk/methods/message_constructor.hpp"

#include <gtest/gtest.h>

TEST(api, example_api_request)
{
    constexpr size_t test_chat_id = vk::api_constants::chat_id_constant + 1;

    vk::method::group_constructor constructor;
    constructor.method("messages.send")
        .param("peer_id", std::to_string(test_chat_id))
        .param("text", "methods::method_construct")
        .perform_request();
}
