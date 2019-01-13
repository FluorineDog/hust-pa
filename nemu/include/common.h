#pragma once
#define DEBUG

#ifdef DEBUG
#define DIFF_TEST
#endif

#if _SHARE
// do not enable these features while building a reference design
#undef DIFF_TEST
#undef DEBUG
#endif

/* You will define this macro in PA2 */
#define HAS_IOE

#include <stdint.h>
#include <assert.h>
#include <string.h>

// typedef uint8_t bool;

typedef uint32_t rtlreg_t;

typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

typedef uint16_t ioaddr_t;
typedef uint32_t ioaddr32_t;
template <typename To>
inline void BITCAST(To& dst, const void* src) {
    dst = (To)src;
}

inline constexpr uint32_t maskify(int bit) {
    if(bit == 32){
        return 0xFFFFFFFF;
    }
    
    return (1U << bit) - 1;
}

#include "debug.h"
#include <optional>
#include <string_view>
#include <tuple>

