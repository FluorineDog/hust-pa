#pragma once
#include "nemu.h"

extern bool g_diff_test_enabled;

#define DIFFTEST_REG_SIZE (sizeof(uint32_t) * 10) // GRPs + EIP

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

void difftest_recover(void);