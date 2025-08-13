#pragma once

#include <cstdint>

#if defined(_MSC_VER)
#include <intrin.h>

#pragma warning(push)
#pragma warning(disable : 4244) // Suppress narrowing warnings

inline int popcount64(uint64_t x) {
    return __popcnt64(x);
}

inline int ctz64(uint64_t x) {
    unsigned long index;
    return _BitScanForward64(&index, x) ? index : 64;
}

#pragma warning(pop)

#elif defined(__GNUC__) || defined(__clang__)

inline int popcount64(uint64_t x) {
    return __builtin_popcountll(x);
}

inline int ctz64(uint64_t x) {
    return x ? __builtin_ctzll(x) : 64;
}

#else
#error "Unsupported compiler for bitintrinsics"
#endif