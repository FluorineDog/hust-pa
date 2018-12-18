#pragma once

#include "nemu.h"

#define make_EHelper(name) void concat(exec_, name) (vaddr_t *eip)
typedef void (*EHelper) (vaddr_t *);

#include "cpu/decode.h"
// DOG: fetch inst and forward eip, and no more than 4byte???
static inline uint32_t instr_fetch(vaddr_t *eip, int len) {
  uint32_t instr = vaddr_read(*eip, len);
#ifdef DEBUG
  uint8_t *p_instr = (uint8_t *)&instr;
  int i;
  for (i = 0; i < len; i ++) {
    g_decoding.p += sprintf(g_decoding.p, "%02x ", p_instr[i]);
  }
#endif
  (*eip) += len;
  return instr;
}

#ifdef DEBUG
#define print_asm(...) Assert(snprintf(g_decoding.assembly, 80, __VA_ARGS__) < 80, "buffer overflow!")
#else
#define print_asm(...)
#endif

#define suffix_char(width) ((width) == 4 ? 'l' : ((width) == 1 ? 'b' : ((width) == 2 ? 'w' : '?')))

#define print_asm_template1(instr) \
  print_asm(str(instr) "%c %s", suffix_char(id_dest->width), id_dest->str)

#define print_asm_template2(instr) \
  print_asm(str(instr) "%c %s,%s", suffix_char(id_dest->width), id_src->str, id_dest->str)

#define print_asm_template3(instr) \
  print_asm(str(instr) "%c %s,%s,%s", suffix_char(id_dest->width), id_src->str, id_src2->str, id_dest->str)

