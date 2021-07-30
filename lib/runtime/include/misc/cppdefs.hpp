#ifndef VK_MISC_CPPDEFS_HPP
#define VK_MISC_CPPDEFS_HPP

#if defined(__GNUC__)
#   define _DECL_VK_EXPORT __attribute__((visibility("default")))
#   define _DECL_VK_IMPORT __attribute__((visibility("default")))
#   define _DECL_VK_HIDDEN __attribute__((visibility("hidden")))
#elif defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(WIN32) || defined(_WIN32)
#   define _DECL_VK_EXPORT __declspec(dllexport)
#   define _DECL_VK_IMPORT __declspec(dllimport)
#   define _DECL_VK_HIDDEN
#else
#   warning Unknown dynamic link import/export semantics.
#   define _DECL_VK_EXPORT
#   define _DECL_VK_IMPORT
#   define _DECL_VK_HIDDEN
#endif

#if defined(VK_MISC_CPPDEFS_HPP)
#   define vk_export _DECL_VK_EXPORT
#else
#   define vk_export _DECL_VK_IMPORT
#endif

#define vk_hidden _DECL_VK_HIDDEN

#define VK_PRAGMA(P) _Pragma(#P)
#define VK_PUSH_DISABLE_WARNINGS _Pragma("GCC diagnostic push")
#define VK_POP_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")
#define VK_DISABLE_GCC_WARNING(WARNING) VK_PRAGMA(GCC diagnostic ignored #WARNING)
#define VK_PUSH_DISABLE_ALL_WARNINGS                                \
    VK_PUSH_DISABLE_WARNINGS                                        \
    VK_DISABLE_GCC_WARNING(-Weffc++)                                \
    VK_DISABLE_GCC_WARNING(-Wall)                                   \
    VK_DISABLE_GCC_WARNING(-Wconversion)                            \
    VK_DISABLE_GCC_WARNING(-Wold - style - cast)                    \
    VK_DISABLE_GCC_WARNING(-Wextra)                                 \
    VK_DISABLE_GCC_WARNING(-Wattributes)                            \
    VK_DISABLE_GCC_WARNING(-Wimplicit - fallthrough)                \
    VK_DISABLE_GCC_WARNING(-Wnon - virtual - dtor)                  \
    VK_DISABLE_GCC_WARNING(-Wreturn - type)                         \
    VK_DISABLE_GCC_WARNING(-Wshadow)                                \
    VK_DISABLE_GCC_WARNING(-Wunused - parameter)                    \
    VK_DISABLE_GCC_WARNING(-Wunused - variable)

#define VK_REALLY_INLINE    inline __attribute__((always_inline))
#define VK_NEVER_INLINE     inline __attribute__((noinline))

#define VK_LIKELY(expr)     __builtin_expect(!!(expr), 1)
#define VK_UNLIKELY(expr)   __builtin_expect(!!(expr), 0)

#define VK_UNUSED(expr)     (void)expr

#define VK_DISABLE_COPY(x)      \
    x(const x&) = delete;       \
    x& operator=(const x&) = delete;

#define VK_DISABLE_MOVE(x)      \
    x(x&&) = delete;            \
    x& operator=(x&&) = delete;

#define VK_DISABLE_COPY_MOVE(x) \
    VK_DISABLE_COPY(x)          \
    VK_DISABLE_MOVE(x)

#endif// VK_MISC_CPPDEFS_HPP
