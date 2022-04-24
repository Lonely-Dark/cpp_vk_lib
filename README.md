[![Ubuntu](https://github.com/epoll-reactor/cpp_vk_lib/workflows/Ubuntu/badge.svg)](https://github.com/epoll-reactor/cpp_vk_lib/actions?query=workflow%3AUbuntu)
[![MacOS](https://github.com/epoll-reactor/cpp_vk_lib/workflows/MacOS/badge.svg)](https://github.com/epoll-reactor/cpp_vk_lib/actions?query=workflow%3AMacOS)
[![CodeFactor](https://www.codefactor.io/repository/github/epoll-reactor/cpp_vk_lib/badge/master)](https://www.codefactor.io/repository/github/epoll-reactor/cpp_vk_lib/overview/master)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://epoll-reactor.github.io/cpp_vk_lib/index.html)

# cpp_vk_lib
![alt text](https://github.com/epoll-reactor/cpp_vk_lib/blob/master/images/vkcpplib-banner-github.png?raw=true)

VK API library

## What is it?
This is a quite compact toolkit for VK. So with this you can easily create your bots and utilities.

[Here](https://github.com/epoll-reactor/template_cpp_vk_bot) is placed example project based on cpp_vk_lib, where
you can learn how to embed it to your CMake project.

## Key features
* Easy API for methods;
* multi-level logging.

## Tested under
Linux, MacOS, FreeBSD, Android.

## Our team
* **[epoll-reactor](https://github.com/epoll-reactor)** - author;
* **[vicsave](https://github.com/vicsave)** - library logo;
* **[edgjj](https://github.com/edgjj)** - cURL download/upload using in-memory buffers.

## Sample

### Note
you can find an example of config in `json_schema/sample_config.json`.
```cpp
#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/runtime/signal_handlers.hpp"
#include "cpp_vk_lib/vk/long_poll/long_poll.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/events/message_new.hpp"
#include "cpp_vk_lib/vk/methods/basic.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2) {						
        std::cerr << "Usage: ./long_poll <config.json>" << std::endl;
        return 1;
    }

    vk::config::load(argv[1]);
    runtime::setup_logger(spdlog::level::level_enum::trace);

    asio::io_context io_context;
    vk::long_poll api(io_context);

    api.on_event(vk::event::type::message_new, [](const vk::event::common& event) {
        vk::event::message_new message = event.get_message_new();
        vk::method::messages::send(message.peer_id(), "response");
    });
    api.run();
}
```
