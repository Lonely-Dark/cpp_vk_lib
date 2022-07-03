#ifndef VK_API_CONSTANTS_HPP
#define VK_API_CONSTANTS_HPP

#include <cstdint>

namespace vk::api_constants {

inline constexpr char api_version[] = "5.130";

inline constexpr char oauth_link[] = "https://oauth.vk.com/";

inline constexpr char android_app_client_secret[] = "hHbZxrka2uZ6jB1inYsH";
inline constexpr char iphone_app_client_secret[] = "VeWdmVclDCtn6ihuP1nt";
inline constexpr char windows_app_client_secret[] = "AlVXZFMUqyrnABp8ncuU";

inline constexpr int32_t android_app_client_id = 2274003;
inline constexpr int32_t windows_app_client_id = 3697615;
inline constexpr int32_t iphone_app_client_id = 3140623;

inline constexpr uint64_t chat_id_constant = 2000000000;

}// namespace vk::api_constants

#endif// VK_API_CONSTANTS_HPP
