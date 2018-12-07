#include "nemu.h"
#include "diff-test.h"

void cpu_exec(uint64_t);
extern "C"
void difftest_memcpy_from_dut(paddr_t dest, void *src, size_t n) {
  memcpy(guest_to_host(dest), src, n);
}

extern "C"
void difftest_getregs(void *r) {
  memcpy(r, &cpu, DIFFTEST_REG_SIZE);
}

extern "C"
void difftest_setregs(const void *r) {
  memcpy(&cpu, r, DIFFTEST_REG_SIZE);
}

extern "C"
void difftest_exec(uint64_t n) {
  cpu_exec(n);
}

extern "C"
void difftest_init(void) {
}
