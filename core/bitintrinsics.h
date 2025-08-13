#pragma once

#if defined(_MSC_VER)
#include <intrin.h>

inline int popcount64(unsigned __int64 x) {
    return __popcnt64(x);
}

inline int ctz64(unsigned __int64 x) {
    unsigned long index;
    return _BitScanForward64(&index, x) ? index : 64;
}

#elif defined(__GNUC__) || defined(__clang__)
inline int popcount64(unsigned long long x) {
    return __builtin_popcountll(x);
}

inline int ctz64(unsigned long long x) {
    return __builtin_ctzll(x);
}

#else
#error "Unsupported compiler for bitintrinsics"
#endif