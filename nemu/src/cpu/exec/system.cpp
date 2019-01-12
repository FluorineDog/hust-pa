#include <nemu.h>
#include "cpu/exec.h"
#include "cpu/intr.h"

void difftest_skip_ref();

void difftest_skip_dut();

make_EHelper(lidt) {
	rtlreg_t base_addr;
	rtl_lm(&cpu.idtr.limit, &id_dest->addr, 2);
	rtl_addi(&base_addr, &id_dest->addr, 2);
	rtl_lm(&cpu.idtr.base, &base_addr, 4);
	print_asm_template1(lidt);
}

make_EHelper(cli) {
	rtl_andi(&cpu.eflags, &cpu.eflags, ~EFLAGS::MASK_IF);
	print_asm_template1(cli);
}

make_EHelper(sti) {
	rtl_ori(&cpu.eflags, &cpu.eflags, EFLAGS::MASK_IF);
	print_asm_template1(sti);
}

// use mov_E2r
make_EHelper(mov_r2cr) {
	assert(id_src->type == OP_TYPE_REG);
	assert(id_dest->type == OP_TYPE_REG);
	rtl_mv(&cpu.ctlreg[id_dest->reg], &id_src->val);
	print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
	// TODO: diff-test
}

// use lea_r2m
make_EHelper(mov_cr2r) {
	assert(id_src->type == OP_TYPE_REG);
	assert(id_dest->type == OP_TYPE_REG);
	operand_write(id_dest, &cpu.ctlreg[id_src->reg]);
	print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));
	// TODO: diff-test
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
	return_from_intr();
	
	print_asm("iret");
}

make_EHelper(in) {
	// JIT_TODO WITH IO
	rtlreg_t dat;
//	dat = pio_read_common((ioaddr_t) id_src->val, rtl_width);
	
	rtlreg_t ioaddr;
	rtl_mv(&ioaddr, &id_src->val);
	rtl_io_in(&dat, &ioaddr, rtl_width);

	operand_write(id_dest, &dat);
    if(dat != 0){
        printf("<<%x>>\n", dat);
        void info_register(); 
        info_register(); 
    }
	print_asm_template2(in);

#if defined(DIFF_TEST)
	difftest_skip_ref();
#endif
}

make_EHelper(out) {
	// JIT_TODO WITH IO
//	pio_write_common((ioaddr_t) id_dest->val, id_src->val, rtl_width);
	rtlreg_t ioaddr;
	rtl_mv(&ioaddr, &id_dest->val);
	rtl_io_out(&ioaddr, &id_src->val, rtl_width);
	print_asm_template2(out);

#if defined(DIFF_TEST)
	difftest_skip_ref();
#endif
}
