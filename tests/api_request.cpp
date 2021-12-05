#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/api_constants.hpp"
#include "cpp_vk_lib/vk/methods/message_constructor.hpp"

int main(int argc, char* argv[])
{
    SECTION(api)
    {
        setup_cpp_vk_lib_environment();

        constexpr size_t test_chat_id = vk::api_constants::chat_id_constant + 1;

        vk::method::group_constructor constructor;
        constructor.method("messages.send")
            .param("peer_id", std::to_string(test_chat_id))
            .param("text",    "methods::method_construct")
            .perform_request();
    }
    return EXIT_SUCCESS;
}
