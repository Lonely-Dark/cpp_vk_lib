#include "attachment/audio.cpp"
#include "attachment/audio_message.cpp"
#include "attachment/base.cpp"
#include "attachment/document.cpp"
#include "attachment/photo.cpp"
#include "attachment/video.cpp"
#include "attachment/wall.cpp"
#include "config/config.cpp"
#include "error/error_code.cpp"
#include "error/exception.cpp"
#include "error/ensure_api_request_succeeded.cpp"
#include "events/attachment_handler.cpp"
#include "events/common.cpp"
#include "events/message_new.cpp"
#include "events/wall_post_new.cpp"
#include "events/wall_reply_new.cpp"
#include "events/wall_repost.cpp"
#include "keyboard/layout.cpp"
#include "long_poll/long_poll.cpp"
#include "methods/basic.cpp"
#include "methods/constructor.cpp"
#include "methods/message_constructor.cpp"
#include "oauth/client.cpp"
