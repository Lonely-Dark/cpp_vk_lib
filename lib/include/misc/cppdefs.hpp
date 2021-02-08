#ifndef VK_CPPDEFS_H
#define VK_CPPDEFS_H

#if defined (__GNUC__)
#   define _DECL_VK_EXPORT __attribute__((visibility("default")))
#   define _DECL_VK_IMPORT __attribute__((visibility("default")))
#   define _DECL_VK_HIDDEN __attribute__((visibility("hidden")))
#elif defined (_MSC_VER) || defined (WIN64) || defined (_WIN64) || defined (WIN32) || defined (_WIN32)
#   define _DECL_VK_EXPORT __declspec(dllexport)
#   define _DECL_VK_IMPORT __declspec(dllimport)
#   define _DECL_VK_HIDDEN
#else
#   warning Unknown dynamic link import/export semantics.
#   define _DECL_VK_EXPORT
#   define _DECL_VK_IMPORT
#   define _DECL_VK_HIDDEN
#endif

#if defined(VK_CPPDEFS_H)
#   define VK_EXPORT _DECL_VK_EXPORT
#else
#   define VK_EXPORT _DECL_VK_IMPORT
#endif
#   define VK_HIDDEN _DECL_VK_HIDDEN

#define VK_PRAGMA(P) _Pragma(#P)
#define VK_PUSH_DISABLE_WARNINGS _Pragma("GCC diagnostic push")
#define VK_POP_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")
#define VK_DISABLE_GCC_WARNING(WARNING) VK_PRAGMA(GCC diagnostic ignored #WARNING)
#define VK_PUSH_DISABLE_ALL_WARNINGS VK_PUSH_DISABLE_WARNINGS \
   VK_DISABLE_GCC_WARNING(-Weffc++) \
   VK_DISABLE_GCC_WARNING(-Wall) \
   VK_DISABLE_GCC_WARNING(-Wconversion) \
   VK_DISABLE_GCC_WARNING(-Wextra) \
   VK_DISABLE_GCC_WARNING(-Wattributes) \
   VK_DISABLE_GCC_WARNING(-Wimplicit-fallthrough) \
   VK_DISABLE_GCC_WARNING(-Wnon-virtual-dtor) \
   VK_DISABLE_GCC_WARNING(-Wreturn-type) \
   VK_DISABLE_GCC_WARNING(-Wshadow) \
   VK_DISABLE_GCC_WARNING(-Wunused-parameter) \
   VK_DISABLE_GCC_WARNING(-Wunused-variable)

#define DISABLE_COPY(type)                      \
    type(const type&) = delete;                 \
    type& operator = (const type&) = delete;    \

#define DISABLE_MOVE(type)                      \
    type(type&&) = delete;                      \
    type& operator = (type&&) = delete;         \

#define DEBUG_WATCH(LABEL, EXPR) \
    std::cerr << #LABEL << ": " << EXPR << std::endl;

#define DEBUG_TRACE(expr) \
    std::cerr << __FILE__ << ':' << __LINE__ << ": <" << __FUNCTION__ << "> " << #expr << " = " << expr << std::endl;

#define VK_ALWAYS_INLINE [[gnu::always_inline]]

#endif // VK_CPPDEFS_H
