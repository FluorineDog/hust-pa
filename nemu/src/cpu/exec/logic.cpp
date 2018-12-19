#include "cpu/exec.h"
#include "cpu/cc.h"

void logical_update_XF(const rtlreg_t* res) {
  rtl_update_ZFSF(res);
  rtl_clear_OF();
  rtl_clear_CF();
}

static inline void test_and_common_ref(rtlreg_t& res){
  rtl_and(&res, &id_dest->val, &id_src->val);
  logical_update_XF(&res);
}

make_EHelper(test) {
	rtlreg_t res;
  test_and_common_ref(res);
  print_asm_template2(test);
}


make_EHelper(and) {
  rtlreg_t res;
  test_and_common_ref(res);
  operand_write(id_dest, &res);
  
  print_asm_template2(and);
}

make_EHelper(xor) {
  rtlreg_t res;
  rtl_xor(&res, &id_dest->val, &id_src->val);
  logical_update_XF(&res);
  operand_write(id_dest, &res);
  
  print_asm_template2(xor);
}

make_EHelper(or) {
  rtlreg_t res;
  rtl_or(&res, &id_dest->val, &id_src->val);
  logical_update_XF(&res);
  operand_write(id_dest, &res);

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = g_decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
