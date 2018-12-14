#pragma once
#include <cstddef>
union EFlags {
	struct {
		unsigned int CF:1; // 0
		unsigned int   :1;
		unsigned int PF:1;
		unsigned int   :1;
		
		unsigned int AF:1; // 4
		unsigned int   :1;
		unsigned int ZF:1;
		unsigned int SF:1;
		
		unsigned int TF:1; // 8
		unsigned int IF:1;
		unsigned int DF:1;
		unsigned int OF:1;
		
		unsigned int IOPL:2; // 12
		unsigned int NT:1;
		unsigned int :1;
		
		unsigned int :16;
	};
	unsigned value;
};

#define eflags_accept_(XX) \
    XX(CF, 0, 1) \
    XX(_plac_hldr1, 1, 1) \
    XX(PF, 2, 1) \
    XX(_plac_hldr2, 3, 1) \
    XX(AF, 4, 1) \
    XX(_plac_hldr3, 5, 1) \
    XX(ZF, 6, 1) \
    XX(SF, 7, 1) \
    XX(TF, 8, 1) \
    XX(IF, 9, 1) \
    XX(DF, 10, 1) \
    XX(OF, 11, 1) \
    XX(IOPL, 12, 2) \
    XX(NT, 14, 1) \
    XX(_plac_hldr4, 15, 1) \
    XX(_plac_hldr5, 16, 16) \
//

#define XX_(name, loc, bit) constexpr int EFLAGS_OFFSET_ ## name = loc;
#define XX(name, loc, bit) XX_(name, loc, bit)
eflags_accept_(XX)
#undef XX_
#undef XX

#define XX_(name, loc, bit) constexpr uint32_t EFLAGS_LOWMASK_ ## name = (1U << bit) - 1;
#define XX(name, loc, bit) XX_(name, loc, bit)
eflags_accept_(XX)
#undef XX_
#undef XX


#define XX_(name, loc, bit) constexpr uint32_t EFLAGS_MASK_ ## name = (EFLAGS_LOWMASK_ ## name) << loc;
#define XX(name, loc, bit) XX_(name, loc, bit)
eflags_accept_(XX)
#undef XX_
#undef XX

