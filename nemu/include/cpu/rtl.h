#pragma once

#include "nemu.h"
#include "util/c_op.h"
#include "cpu/relop.h"
#include "cpu/rtl-wrapper.h"
#include "eflags.h"

extern rtlreg_t t0, t1, t2, t3;
extern int &rtl_width;

void decoding_set_jmp(bool is_jmp);

bool internal_relop(uint32_t relop, const rtlreg_t src1, const rtlreg_t src2, int width);

/* RTL basic instructions */

void jit_rtl_li(rtlreg_t *dest, uint32_t imm);


void jit_rtl_mv(rtlreg_t *dest, const rtlreg_t *src1);
#include "arith.h"


void jit_rtl_div64_q(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2);

void jit_rtl_div64_r(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2);

void jit_rtl_idiv64_q(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2);

void jit_rtl_idiv64_r(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2);

void jit_rtl_lm(rtlreg_t *dest, const rtlreg_t *addr, int len);
void jit_rtl_sm(const rtlreg_t *addr, const rtlreg_t *src1, int len);
void jit_rtl_host_lm(rtlreg_t *dest, const void *addr, int len);
void jit_rtl_host_sm(void *addr, const rtlreg_t *src1, int len);


void jit_rtl_setrelop(uint32_t relop, rtlreg_t *dest,
		const rtlreg_t *src1, const rtlreg_t *src2);

void jit_rtl_j(vaddr_t target);
void jit_rtl_jr(rtlreg_t *target);

void jit_rtl_jcond(const rtlreg_t *cond, vaddr_t target);


void jit_rtl_exit(int state);

/// jit finish






/* RTL pseudo instructions */

static inline void rtl_lr(rtlreg_t *dest, int r, int width) {
	switch (width) {
		case 4:
			rtl_mv(dest, &reg_l(r));
			return;
		case 1:
			rtl_host_lm(dest, &reg_b(r), 1);
			return;
		case 2:
			rtl_host_lm(dest, &reg_w(r), 2);
			return;
		default:
			panic("wtf");
	}
}

static inline void rtl_sr(int r, const rtlreg_t *src1, int width) {
	switch (width) {
		case 4:
			rtl_mv(&reg_l(r), src1);
			return;
		case 1:
			rtl_host_sm(&reg_b(r), src1, 1);
			return;
		case 2:
			rtl_host_sm(&reg_w(r), src1, 2);
			return;
		default:
			panic("wtf");
	}
}

static inline void rtl_not(rtlreg_t *dest, const rtlreg_t *src1) {
	// dest <- ~src1
	rtl_li(dest, -1);
	rtl_sub(dest, dest, src1);
	// TODO();
}

static inline void rtl_sext(rtlreg_t *dest, const rtlreg_t *src1, int width) {
	// dest <- signext(src1[(width * 8 - 1) .. 0])
	int offset = (4 - width) * 8;
	rtl_shli(dest, src1, offset);
	rtl_sari(dest, dest, offset);
	// TODO();
}

static inline void rtl_push(const rtlreg_t *src1) {
	// esp <- esp - 4
	// M[esp] <- src1
	rtl_subi(&cpu.esp, &cpu.esp, 4);
	rtl_sm(&cpu.esp, src1, 4);
	//   TODO();
}

static inline void rtl_pop(rtlreg_t *dest) {
	// dest <- M[esp]
	// esp <- esp + 4
	rtl_lm(dest, &cpu.esp, 4);
	rtlreg_t imm;
	rtl_li(&imm, 0xcccccccc);
	rtl_sm(&cpu.esp, &imm, 4);
	rtl_addi(&cpu.esp, &cpu.esp, 4);
	//   TODO();
}

static inline void rtl_setrelopi(uint32_t relop, rtlreg_t *dest, const rtlreg_t *src1,
		int imm) {
	// dest <- (src1 relop imm ? 1 : 0)
	rtlreg_t imm_at;
	rtl_li(&imm_at, imm);
	rtl_setrelop(relop, dest, src1, &imm_at);
	//   TODO();
}

static inline void rtl_msb(rtlreg_t *dest, const rtlreg_t *src1, int width) {
	// dest <- src1[width * 8 - 1]
	//   int offset = width * 8;
	//   rtl_shri(dest, src1, )
	int offset = (width * 8 - 1);
	rtl_shri(dest, src1, offset);
	rtl_andi(dest, dest, 0x1);
	// TODO();
}

#define make_rtl_setget_eflags(f)                                            \
    static inline void name_concat(rtl_update_bit_, f)(const rtlreg_t *src_bit) { \
        using namespace EFLAGS;                                              \
        g_ignore_eflags &= ~MASK_##f;                                        \
        rtlreg_t bitmask;                                                    \
        auto OFFSET = OFFSET_##f;                                            \
        auto CL_MASK = ~(MASK_##f);                                          \
        rtl_shli(&bitmask, src_bit, OFFSET);                                 \
        rtl_andi(&cpu.eflags, &cpu.eflags, CL_MASK);                         \
        rtl_or(&cpu.eflags, &cpu.eflags, &bitmask);                          \
        /*TODO();*/                                                          \
    }                                                                        \
    static inline void name_concat(rtl_get_, f)(rtlreg_t * dest) {                \
        using namespace EFLAGS;                                              \
        auto OFFSET = OFFSET_##f;                                            \
        auto LOWMASK = LOWMASK_##f;                                          \
        rtl_shri(dest, &cpu.eflags, OFFSET);                                 \
        rtl_andi(dest, dest, LOWMASK);                                       \
        /*TODO();*/                                                          \
    }                                                                        \
    static inline void name_concat(rtl_set_, f)() {                               \
        using namespace EFLAGS;                                              \
        g_ignore_eflags &= ~MASK_##f;                                        \
        rtl_ori(&cpu.eflags, &cpu.eflags, MASK_##f);                         \
        /*TODO();*/                                                          \
    }                                                                        \
    static inline void name_concat(rtl_clear_, f)() {                             \
        using namespace EFLAGS;                                              \
        rtl_andi(&cpu.eflags, &cpu.eflags, ~MASK_##f);                       \
        /*TODO();*/                                                          \
    }

make_rtl_setget_eflags(CF)

make_rtl_setget_eflags(OF)

make_rtl_setget_eflags(ZF)

make_rtl_setget_eflags(SF)

static inline void rtl_fullupdate_ZF(const rtlreg_t *result) {
	// eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
	rtlreg_t res_at;
	rtl_setrelopi(RELOP_EQ, &res_at, result, 0);
	rtl_update_bit_ZF(&res_at);
	//  TODO();
}

static inline void rtl_fullupdate_SF(const rtlreg_t *result) {
	// eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
	rtlreg_t res_at;
	rtl_setrelopi(RELOP_LT, &res_at, result, 0);
	rtl_update_bit_SF(&res_at);
	//  TODO();
}

static inline void rtl_update_ZFSF(const rtlreg_t *result) {
	rtl_fullupdate_ZF(result);
	rtl_fullupdate_SF(result);
}

static inline void rtl_cond(rtlreg_t *res, const rtlreg_t *cond, const rtlreg_t *yes,
		const rtlreg_t *no) {
	rtlreg_t yesmask, nomask;
	rtlreg_t bit;
	rtl_setrelopi(RELOP_NE, &bit, cond, 0);
	rtl_subi(&nomask, &bit, 1);
	rtl_not(&yesmask, &nomask);
	rtl_and(&yesmask, &yesmask, yes);
	rtl_and(&nomask, &nomask, no);
	rtl_or(res, &yesmask, &nomask);
}

static inline void rtl_testbit(rtlreg_t *dest, const rtlreg_t *src, int bit_offset) {
	rtl_shri(dest, src, bit_offset);
	rtl_andi(dest, dest, 0x1);
}

