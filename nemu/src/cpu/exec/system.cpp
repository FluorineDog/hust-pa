#include <nemu.h>
#include "cpu/exec.h"
#include "device/port-io.h"
#include "cpu/intr.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
    //   TODO();
    rtlreg_t base_addr;
    rtl_lm(&cpu.idtr.limit, &id_dest->addr, 2);
    rtl_addi(&base_addr, &id_dest->addr, 2);
    rtl_lm(&cpu.idtr.base, &base_addr, 4);
    print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
    TODO();

    print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
    TODO();

    print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
    difftest_skip_ref();
#endif
}

make_EHelper(int) {
	assert(id_dest->type == OP_TYPE_IMM);
	raise_intr(id_dest->val, g_decoding.seq_eip);
    print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
    difftest_skip_dut();
#endif
}

make_EHelper(iret) {
    TODO();

    print_asm("iret");
}

make_EHelper(in) {
    // JIT_TODO WITH IO
    rtlreg_t dat;
    dat = pio_read_common((ioaddr_t)id_src->val, rtl_width);
    operand_write(id_dest, &dat);
    print_asm_template2(in);

#if defined(DIFF_TEST)
    difftest_skip_ref();
#endif
}

make_EHelper(out) {
    //  TODO();
    pio_write_common((ioaddr_t)id_dest->val, id_src->val, rtl_width);
    print_asm_template2(out);

#if defined(DIFF_TEST)
    difftest_skip_ref();
#endif
}
