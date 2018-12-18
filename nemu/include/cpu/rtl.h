#pragma once
#include "nemu.h"
#include "util/c_op.h"
#include "cpu/relop.h"
#include "cpu/rtl-wrapper.h"
#include "eflags.h"

extern rtlreg_t t0, t1, t2, t3;

void decoding_set_jmp(bool is_jmp);
bool interpret_relop(uint32_t relop, const rtlreg_t src1, const rtlreg_t src2);

/* RTL basic instructions */

static inline void interpret_rtl_li(rtlreg_t* dest, uint32_t imm) {
  *dest = imm;
}

static inline void interpret_rtl_mv(rtlreg_t* dest, const rtlreg_t *src1) {
  *dest = *src1;
}

#define make_rtl_arith_logic(name) \
  static inline void concat(interpret_rtl_, name) (rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2) { \
    *dest = concat(c_, name) (*src1, *src2); \
  } \
  /* Actually those of imm version are pseudo rtl instructions,
   * but we define them here in the same macro */ \
  static inline void concat(rtl_, name ## i) (rtlreg_t* dest, const rtlreg_t* src1, int imm) { \
    rtlreg_t imm_at; \
    rtl_li(&imm_at, imm); \
    rtl_ ## name (dest, src1, &imm_at); \
  }

make_rtl_arith_logic(add)
make_rtl_arith_logic(sub)
make_rtl_arith_logic(and)
make_rtl_arith_logic(or)
make_rtl_arith_logic(xor)
make_rtl_arith_logic(shl)
make_rtl_arith_logic(shr)
make_rtl_arith_logic(sar)
make_rtl_arith_logic(mul_lo)
make_rtl_arith_logic(mul_hi)
make_rtl_arith_logic(imul_lo)
make_rtl_arith_logic(imul_hi)
make_rtl_arith_logic(div_q)
make_rtl_arith_logic(div_r)
make_rtl_arith_logic(idiv_q)
make_rtl_arith_logic(idiv_r)

static inline void interpret_rtl_div64_q(rtlreg_t* dest,
    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2) {
  uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
  uint32_t divisor = (*src2);
  *dest = dividend / divisor;
}

static inline void interpret_rtl_div64_r(rtlreg_t* dest,
    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2) {
  uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
  uint32_t divisor = (*src2);
  *dest = dividend % divisor;
}

static inline void interpret_rtl_idiv64_q(rtlreg_t* dest,
    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2) {
  int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
  int32_t divisor = (*src2);
  *dest = dividend / divisor;
}

static inline void interpret_rtl_idiv64_r(rtlreg_t* dest,
    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2) {
  int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
  int32_t divisor = (*src2);
  *dest = dividend % divisor;
}

static inline void interpret_rtl_lm(rtlreg_t *dest, const rtlreg_t* addr, int len) {
  *dest = vaddr_read(*addr, len);
}

static inline void interpret_rtl_sm(const rtlreg_t* addr, const rtlreg_t* src1, int len) {
  vaddr_write(*addr, *src1, len);
}

static inline void interpret_rtl_host_lm(rtlreg_t* dest, const void *addr, int len) {
  switch (len) {
    case 4: *dest = *(uint32_t *)addr; return;
    case 1: *dest = *( uint8_t *)addr; return;
    case 2: *dest = *(uint16_t *)addr; return;
    default: assert(0);
  }
}

static inline void interpret_rtl_host_sm(void *addr, const rtlreg_t *src1, int len) {
  switch (len) {
    case 4: *(uint32_t *)addr = *src1; return;
    case 1: *( uint8_t *)addr = *src1; return;
    case 2: *(uint16_t *)addr = *src1; return;
    default: assert(0);
  }
}

static inline void interpret_rtl_setrelop(uint32_t relop, rtlreg_t *dest,
    const rtlreg_t *src1, const rtlreg_t *src2) {
  *dest = interpret_relop(relop, *src1, *src2);
}

static inline void interpret_rtl_j(vaddr_t target) {
  cpu.eip = target;
  decoding_set_jmp(true);
}

static inline void interpret_rtl_jr(rtlreg_t *target) {
  // very hard to use
  cpu.eip = *target;
  decoding_set_jmp(true);
}

static inline void interpret_rtl_jrelop(uint32_t relop,
    const rtlreg_t *src1, const rtlreg_t *src2, vaddr_t target) {
  bool is_jmp = interpret_relop(relop, *src1, *src2);
  if (is_jmp) cpu.eip = target;
  decoding_set_jmp(is_jmp);
}

void interpret_rtl_exit(int state);


/* RTL pseudo instructions */

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(dest, &reg_l(r)); return;
    case 1: rtl_host_lm(dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline void rtl_sr(int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(&reg_l(r), src1); return;
    case 1: rtl_host_sm(&reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(&reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline void rtl_not(rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- ~src1
  rtl_li(dest, -1);
  rtl_sub(dest, dest, src1);
  // TODO();
}

static inline void rtl_sext(rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- signext(src1[(width * 8 - 1) .. 0])
  int offset = (4 - width) * 8;
  rtl_shli(dest, src1, offset);
  rtl_sari(dest, dest, offset);
  // TODO();
}

static inline void rtl_push(const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
  rtl_subi(&cpu.esp, &cpu.esp, 4);
  rtl_sm(&cpu.esp, src1, 4);
//   TODO();
}

static inline void rtl_pop(rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
  rtl_lm(dest, &cpu.esp, 4);
  rtl_addi(&cpu.esp, &cpu.esp, 4);
//   TODO();
}

static inline void rtl_setrelopi(uint32_t relop, rtlreg_t *dest,
    const rtlreg_t *src1, int imm) {
  // dest <- (src1 relop imm ? 1 : 0)
  rtlreg_t imm_at;
  rtl_li(&imm_at, imm);
  rtl_setrelop(relop, dest, src1, &imm_at);
//   TODO();
}

static inline void rtl_msb(rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
//   int offset = width * 8;
//   rtl_shri(dest, src1, )
  int offset = (width * 8 - 1);
  rtl_shri(dest, src1, offset);
  rtl_andi(dest, dest, 0x1);
// TODO();
}



#define make_rtl_setget_eflags(f) \
  static inline void concat(rtl_update_, f) (const rtlreg_t* src) { \
    using namespace EFLAGS;  \
    auto OFFSET = OFFSET_ ## f;\
    auto CL_MASK = ~(MASK_ ## f); \
    rtlreg_t bitmask; \
    rtl_setrelopi(RELOP_NE, &bitmask, src, 0);\
    rtl_shli(&bitmask, &bitmask, OFFSET);\
    rtl_andi(&cpu.eflags, &cpu.eflags, CL_MASK);\
    rtl_or(&cpu.eflags, &cpu.eflags, &bitmask); \
    /*TODO();*/ \
  } \
  static inline void concat(rtl_get_, f) (rtlreg_t* dest) { \
    using namespace EFLAGS;  \
    auto OFFSET = OFFSET_ ## f;\
    auto LOWMASK = LOWMASK_ ## f;\
    rtl_shri(dest, &cpu.eflags, OFFSET); \
    rtl_andi(dest, dest, LOWMASK); \
    /*TODO();*/ \
  } \
  static inline void concat(rtl_set_, f) () { \
    using namespace EFLAGS;  \
    rtl_ori(&cpu.eflags, &cpu.eflags, MASK_ ## f); \
    /*TODO();*/ \
  }  \
  static inline void concat(rtl_clear_, f) () { \
    using namespace EFLAGS;  \
    rtl_andi(&cpu.eflags, &cpu.eflags, ~ MASK_ ## f); \
    /*TODO();*/ \
  }
  

make_rtl_setget_eflags(CF)
make_rtl_setget_eflags(OF)
make_rtl_setget_eflags(ZF)
make_rtl_setget_eflags(SF)

static inline void rtl_update_ZF(const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
  rtlreg_t res_at;
  rtl_setrelopi(RELOP_EQ, &res_at, result, 0);
  rtl_update_ZF(&res_at);
//  TODO();
}

static inline void rtl_update_SF(const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  rtlreg_t res_at;
  rtl_setrelopi(RELOP_LT, &res_at, result, 0);
  rtl_update_SF(&res_at);
//  TODO();
}

static inline void rtl_update_ZFSF(const rtlreg_t* result, int width) {
  rtl_update_ZF(result, width);
  rtl_update_SF(result, width);
}

