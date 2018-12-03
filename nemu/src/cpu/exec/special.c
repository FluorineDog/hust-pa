#include "cpu/exec.h"
#include "monitor/monitor.h"

void interpret_rtl_exit(int state) {
  nemu_state = state;
}

make_EHelper(nop) {
  print_asm("nop");
}

make_EHelper(inv) {
  /* invalid opcode */

  uint32_t temp[2];
  vaddr_t ori_eip = cpu.eip;
  *eip = ori_eip;
  temp[0] = instr_fetch(eip, 4);
  temp[1] = instr_fetch(eip, 4);

  uint8_t *p = (void *)temp;
  printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n",
      ori_eip, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

  extern char logo [];
  printf("There are two cases which will trigger this unexpected exception:\n"
      "1. The instruction at eip = 0x%08x is not implemented.\n"
      "2. Something is implemented incorrectly.\n", ori_eip);
  printf("Find this eip(0x%08x) in the disassembling result to distinguish which case it is.\n\n", ori_eip);
  printf("\33[1;31mIf it is the first case, see\n%s\nfor more details.\n\nIf it is the second case, remember:\n"
      "* The machine is always right!\n"
      "* Every line of untested code is always wrong!\33[0m\n\n", logo);

  rtl_exit(NEMU_ABORT);

  print_asm("invalid opcode");
}

make_EHelper(nemu_trap) {
#if defined(DIFF_TEST)
  void difftest_skip_ref();
  difftest_skip_ref();
#endif

  rtl_exit(NEMU_END);

  print_asm("nemu trap");
  return;
}
