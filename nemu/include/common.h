#ifndef __COMMON_H__
#define __COMMON_H__

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
template<typename To>
inline void BITCAST(To& dst, const void* src){
    dst = (To)src; 
}

#include "debug.h"
#include "macro.h"
#include <optional>
#include <string_view>
#include <tuple>

#endif
