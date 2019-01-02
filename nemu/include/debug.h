#pragma once

#include "common.h"
#include <stdio.h>
#include <assert.h>

extern uint64_t g_nr_guest_instr;

#ifdef DEBUG
extern FILE *log_fp;
#define Log_write(format, ...)                      \
    do {                                            \
        if(log_fp != NULL) {                        \
            fprintf(log_fp, format, ##__VA_ARGS__); \
            fflush(log_fp);                         \
        }                                           \
    } while(0)
#else
#define Log_write(format, ...)
#endif

#define printflog(format, ...)            \
    do {                                  \
        printf(format, ##__VA_ARGS__);    \
        fflush(stdout);                   \
        Log_write(format, ##__VA_ARGS__); \
    } while(0)

#define Log(format, ...)                                                              \
    printflog("\33[1;34m[%s,%d,%s] " format "\33[0m\n", __FILE__, __LINE__, __func__, \
              ##__VA_ARGS__)

#define Assert(cond, ...)                 \
    do {                                  \
        if(!(cond)) {                     \
            fflush(stdout);               \
            fprintf(stderr, "\33[1;31m"); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\33[0m\n");  \
            fprintf(stderr, "{{nr=%ld}}", g_nr_guest_instr); \
            assert(cond);                 \
        }                                 \
    } while(0)

#define panic(format, ...)                \
    do {                                  \
        Assert(0, format, ##__VA_ARGS__); \
        __builtin_unreachable();          \
    } while(0)

#define TODO() panic("please implement me")
