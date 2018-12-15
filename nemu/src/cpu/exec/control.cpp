#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(g_decoding.jmp_eip);

  print_asm("jmp %x", g_decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = g_decoding.opcode & 0xf;
  rtl_setcc(&t0, cc);
  rtl_li(&t1, 0);
  rtl_jrelop(RELOP_NE, &t0, &t1, g_decoding.jmp_eip);

  print_asm("j%s %x", get_cc_name(cc), g_decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  rtlreg_t seqEIP;
  rtl_li(&seqEIP, g_decoding.seq_eip);
  rtl_push(&seqEIP);
  rtl_j(g_decoding.jmp_eip);
//  TODO();
  print_asm("call %x", g_decoding.jmp_eip);
}

make_EHelper(ret) {
//  TODO();
  rtlreg_t target_eip;
  rtl_pop(&target_eip);
  rtl_jr(&target_eip);
  print_asm("ret");
}

make_EHelper(call_rm) {
  TODO();

  print_asm("call *%s", id_dest->str);
}
