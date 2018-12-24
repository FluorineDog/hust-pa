#include "cpu/exec.h"

make_EHelper(mov);
make_EHelper(movzx);
make_EHelper(movsx);
make_EHelper(mov_str);
make_EHelper(sto_str);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

// TODO: untested
make_EHelper(call);
make_EHelper(call_rm);

make_EHelper(push);
make_EHelper(pop);

make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(cmp);
make_EHelper(not);
make_EHelper(neg);

make_EHelper(nop);


make_EHelper(xor);
make_EHelper(leave);

make_EHelper(ret);
make_EHelper(ret_imm);

make_EHelper(lea);
make_EHelper(and);
make_EHelper(test);
make_EHelper(or);

make_EHelper(inc);
make_EHelper(dec);

make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(jmp_rm);

make_EHelper(setcc);

make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(imul3);
make_EHelper(mul);
make_EHelper(div);
make_EHelper(idiv);

make_EHelper(cbw_cwde);
make_EHelper(cwd_cdq);

make_EHelper(shl);
make_EHelper(shr);
make_EHelper(sar);

make_EHelper(in);
make_EHelper(out);

make_EHelper(rol);

make_EHelper(lidt);

make_EHelper(int);

make_EHelper(pusha);

