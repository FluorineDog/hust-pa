#pragma once
#include <stddef.h>
#include <stdint.h>

extern uint32_t g_ignore_eflags;
#define eflags_accept_(XX)  \
    XX(CF, 0, 1)            \
    XX(_plac_hldr1, 1, 1)   \
    XX(PF, 2, 1)            \
    XX(_plac_hldr2, 3, 1)   \
    XX(AF, 4, 1)            \
    XX(_plac_hldr3, 5, 1)   \
    XX(ZF, 6, 1)            \
    XX(SF, 7, 1)            \
    XX(TF, 8, 1)            \
    XX(IF, 9, 1)            \
    XX(DF, 10, 1)           \
    XX(OF, 11, 1)           \
    XX(IOPL, 12, 2)         \
    XX(NT, 14, 1)           \
    XX(_plac_hldr4, 15, 1)  \
    XX(_plac_hldr5, 16, 16) \
    //

#define XX_(name, loc, bit) OFFSET_##name = loc,
#define XX(name, loc, bit) XX_(name, loc, bit)
enum { eflags_accept_(XX) };
#undef XX_
#undef XX

#define XX_(name, loc, bit) LOWMASK_##name = (1U << bit) - 1,
#define XX(name, loc, bit) XX_(name, loc, bit)
enum { eflags_accept_(XX) };
#undef XX_
#undef XX

#define XX_(name, loc, bit) MASK_##name = (LOWMASK_##name) << loc,
#define XX(name, loc, bit) XX_(name, loc, bit)
enum { eflags_accept_(XX) };
#undef XX_
#undef XX

#define XX_(name, loc, bit)                                  \
    uint32_t get_##name(uint32_t eflags) {                   \
        return (LOWMASK_##name & (eflags >> OFFSET_##name)); \
    }
#define XX(name, loc, bit) XX_(name, loc, bit)
eflags_accept_(XX)
#undef XX_
#undef XX
