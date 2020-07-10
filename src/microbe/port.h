#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define MICROBE_LIKELY(x) __builtin_expect(!!(x), 1)
#define MICROBE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define MICROBE_ALWAYS_INLINE __attribute__((always_inline))
#define MICROBE_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define MICROBE_LIKELY(x) x
#define MICROBE_UNLIKELY(x) x
#define MICROBE_ALWAYS_INLINE __forceinline
#define MICROBE_UNREACHABLE() __assume(0)
#else
#include <assert.h>
#define MICROBE_LIKELY(x) x
#define MICROBE_UNLIKELY(x) x
#define MICROBE_ALWAYS_INLINE inline
#define MICROBE_UNREACHABLE() assert(!"unreachable")
#endif