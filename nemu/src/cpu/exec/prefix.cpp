#include "cpu/exec.h"

make_EHelper(real);

make_EHelper(operand_size) {
  g_decoding.is_operand_size_16 = true;
  exec_real(eip);
  g_decoding.is_operand_size_16 = false;
}


