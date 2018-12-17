#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
//  TODO();
  assert(id_dest->type == OP_TYPE_REG);
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  TODO();
  assert(id_dest->type == OP_TYPE_REG);
  rtlreg_t tmp;
  rtl_pop(&tmp);
  operand_write(id_dest, &tmp);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (g_decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(g_decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (g_decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(g_decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = g_decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = g_decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  assert(id_src->type == OP_TYPE_MEM);
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
