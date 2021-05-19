#include "methods/video.hpp"

vk::method::video::video()
  : m_user_method()
  , m_document()
{}

vk::method::video::video(std::string_view user_token)
  : m_user_method(user_token.data())
  , m_document(user_token.data())
{}

vk::attachment::attachments_t vk::method::video::search(std::string_view query, std::int64_t count) const
{
    return m_document.search("video.search", query, count);
}

void vk::method::video::save_by_link(std::string_view url) const
{
    m_user_method.impl()
        .method("video.save")
        .param("link", url)
        .execute();
}
