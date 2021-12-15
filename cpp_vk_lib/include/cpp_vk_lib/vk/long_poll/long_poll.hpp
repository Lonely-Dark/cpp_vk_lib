#ifndef VK_LONG_POLL_API_HPP
#define VK_LONG_POLL_API_HPP

#include "asio/io_context.hpp"
#include "asio/post.hpp"
#include "cpp_vk_lib/runtime/misc/cppdefs.hpp"
#include "cpp_vk_lib/runtime/uncopyable.hpp"
#include "cpp_vk_lib/runtime/unmovable.hpp"
#include "cpp_vk_lib/vk/error/error_code.hpp"
#include "cpp_vk_lib/vk/events/common.hpp"
#include "cpp_vk_lib/vk/events/type.hpp"

#include <unordered_map>
#include <vector>

namespace simdjson::dom {
class parser;
}// namespace simdjson::dom

namespace vk {
/*!  Event queue that implements group long polling. */
class long_poll : public runtime::uncopyable, public runtime::unmovable
{
    /*! groups.getLongPollServer wrapper. */
    struct poll_payload
    {
        std::string key;
        std::string server;
        std::string ts;
    };

public:
    /*!
     * Get and setup group_id by token.
     *
     * \throws exception::access_error if group_id retrieving failed
     */
    long_poll(asio::io_context&);

    /*! Destructor to satisfy correct opaque pointer handling. */
    ~long_poll();
    /*!
     * Setup action on selected event type.
     *
     * \note If invalid event type provided, nothing will happen.
     */
    template <typename Executor>
    void on_event(event::type type, Executor executor)
    {
        executors_[type] = executor;
    }
    /*!
     * Start infinite event loop after setup events event handlers using long_poll::on_event.
     * After all tasks are completed, the queue returns to its original state.
     */
    void run(int8_t timeout = 60);

private:
    /*! Get new long poll server. */
    void server();
    /*!
     * Execute "listening" with maximum delay = timeout.
     * At first start, new Long Poll server retrieved.
     * If Long Poll returned error with code 2 or 3, new server is retrieved.
     *
     * \return raw event list
     */
    std::vector<event::common> listen(int8_t timeout = 60);

    /*! Event asynchronous task queue. */
    asio::io_context& io_context_;
    /*! Received information from polling. */
    poll_payload poll_payload_;
    /*! Tracked group id. */
    int64_t group_id_;
    /*! Set of executors, which are called on specified event type. */
    std::unordered_map<event::type, std::function<void(const vk::event::common&)>> executors_;
    /*! Single parser to all methods for better performance. */
    std::unique_ptr<simdjson::dom::parser> shared_parser_;
};

}// namespace vk

#endif// VK_LONG_POLL_API_HPP
