#ifndef __DIFF_TEST_H__
#define __DIFF_TEST_H__
#include "nemu.h"

#define DIFFTEST_REG_SIZE (sizeof(uint32_t) * 9) // GRPs + EIP

extern "C"
void difftest_memcpy_from_dut(paddr_t dest, void *src, size_t n);

extern "C"
void difftest_getregs(void *r);

extern "C"
void difftest_setregs(const void *r);

extern "C"
void difftest_exec(uint64_t n);

extern "C"
void difftest_init(void);


#endif
