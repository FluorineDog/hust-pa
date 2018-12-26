#pragma once
/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
//#define HAS_VME

#include <am.h>
#include <klib.h>
#include "debug.h"

typedef char bool;
#define true 1
#define false 0

#ifndef __unused
#define __unused __attribute__((unused))
#endif    // __unused

// #define TODO() panic("TODO");