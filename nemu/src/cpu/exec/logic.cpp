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
};

make_EHelper(or) {
  rtlreg_t res;
  rtl_or(&res, &id_dest->val, &id_src->val);
  logical_update_XF(&res);
  operand_write(id_dest, &res);

  print_asm_template2(or);
}

inline void shift_issue(const rtlreg_t& res, const rtlreg_t& offset){
  // unnecessary to update CF and OF in NEMU
  using namespace EFLAGS;
  g_ignore_eflags = MASK_CF | MASK_OF;
  rtlreg_t updater;
  rtlreg_t SF, ZF, fake;
  rtl_get_SF(&SF);
  rtl_get_ZF(&ZF);
  rtl_shli(&fake, &SF, rtl_width * 8 - 1);
  rtl_xor(&fake, &fake, &ZF);
  rtl_xori(&fake, &fake, 1);
  // if 0, no update
  rtl_cond(&updater, &offset, &res, &fake);
  rtl_update_ZFSF(&updater);
}

make_EHelper(rol) {
  using namespace EFLAGS;
  g_ignore_eflags |= MASK_OF;
  g_ignore_eflags &= ~MASK_CF;
  // others regarded as TODO
  assert(rtl_width == 4);
  
  rtlreg_t res, left_shift, right_shift, oldCF, curCF, CF, ZERO;
  rtl_get_CF(&oldCF);
  
  rtl_andi(&left_shift, &id_src->val, 0x1F);
  
  rtl_li(&ZERO, 0);
  rtl_sub(&right_shift, &ZERO, &id_src->val);
  rtl_andi(&right_shift, &right_shift, 0x1F);
  
  rtlreg_t ansL, ansR;
  rtl_shl(&ansL, &id_dest->val, &left_shift);
  rtl_shr(&ansR, &id_dest->val, &right_shift);
  
  rtl_or(&res, &ansL, &ansR);
  rtl_andi(&curCF, &res, 0x1);
  rtl_cond(&CF, &left_shift, &curCF, &oldCF);
  rtl_update_bit_CF(&CF);
  operand_write(id_dest, &res);

  print_asm_template2(sar);
}

make_EHelper(sar) {
  
  rtlreg_t res, val, offset;
  rtl_sext(&val, &id_dest->val, rtl_width);
  rtl_andi(&offset, &id_src->val, rtl_width * 8 - 1);
  rtl_sar(&res, &val, &offset);
  shift_issue(res, offset);
  operand_write(id_dest, &res);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  rtlreg_t res, val, offset;
  rtl_andi(&val, &id_dest->val, maskify(8 * rtl_width));
  rtl_andi(&offset, &id_src->val, rtl_width * 8 - 1);
  rtl_shl(&res, &val, &offset);
  shift_issue(res, offset);
  operand_write(id_dest, &res);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtlreg_t res, val, offset;
  rtl_andi(&val, &id_dest->val, maskify(8 * rtl_width));
  rtl_andi(&offset, &id_src->val, rtl_width * 8 - 1);
  rtl_shr(&res, &val, &offset);
  shift_issue(res, offset);
  operand_write(id_dest, &res);


  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = g_decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtlreg_t res;
  rtl_not(&res, &id_dest->val);
  operand_write(id_dest, &res);
  print_asm_template1(not);
}
