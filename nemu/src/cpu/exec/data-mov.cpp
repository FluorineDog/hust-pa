#include <nemu.h>
#include "cpu/exec.h"

make_EHelper(mov) {
	operand_write(id_dest, &id_src->val);
	print_asm_template2(mov);
}

make_EHelper(mov_str) {
	rtlreg_t data;
	assert(!(cpu.eflags & EFLAGS::MASK_DF));
	rtl_lm(&data, (const rtlreg_t *)&cpu.esi, rtl_width);
	rtl_sm(&cpu.edi, (const rtlreg_t *)&data, rtl_width);
	rtl_addi(&cpu.esi, &cpu.esi, rtl_width);
	rtl_addi(&cpu.edi, &cpu.edi, rtl_width);
#ifdef DEBUG
	strcpy(id_dest->str, "%es:%(edi)");
	strcpy(id_src->str, "%ds:%(esi)");
#endif
	
	print_asm_template2(movs);
}


make_EHelper(sto_str) {
	// ALWAYS WRONG
	assert(!(cpu.eflags & EFLAGS::MASK_DF));
	rtl_sm(&cpu.edi, (const rtlreg_t *)&id_src->val, rtl_width);
	rtl_addi(&cpu.edi, &cpu.edi, rtl_width);
#ifdef DEBUG
	strcpy(id_dest->str, "%es:%(edi)");
#endif
	print_asm_template2(stos);
}

make_EHelper(push) {
	rtl_push(&id_dest->val);
	print_asm_template1(push);
}

make_EHelper(pop) {
	rtlreg_t tmp;
	rtl_pop(&tmp);
	operand_write(id_dest, &tmp);
	print_asm_template1(pop);
}

make_EHelper(pusha) {
//	TODO();
	assert(rtl_width == 4);
	rtlreg_t tempESP;
	rtl_mv(&tempESP, &cpu.esp);
	static_assert(&cpu.esp == &cpu.gpr[4]._32);
	rtl_push(&cpu.gpr[0]._32);
	rtl_push(&cpu.gpr[1]._32);
	rtl_push(&cpu.gpr[2]._32);
	rtl_push(&cpu.gpr[3]._32);
	rtl_push(&tempESP);
	rtl_push(&cpu.gpr[5]._32);
	rtl_push(&cpu.gpr[6]._32);
	rtl_push(&cpu.gpr[7]._32);
	print_asm("pusha");
}

make_EHelper(popa) {
	// TODO
	rtlreg_t dummy;
	rtl_pop(&cpu.gpr[7]._32);
	rtl_pop(&cpu.gpr[6]._32);
	rtl_pop(&cpu.gpr[5]._32);
	rtl_pop(&dummy);
	static_assert(&cpu.esp == &cpu.gpr[4]._32);
	rtl_pop(&cpu.gpr[3]._32);
	rtl_pop(&cpu.gpr[2]._32);
	rtl_pop(&cpu.gpr[1]._32);
	rtl_pop(&cpu.gpr[0]._32);
	print_asm("popa");
}

make_EHelper(leave) {
	rtl_mv(&cpu.esp, &cpu.ebp);
	rtl_pop(&cpu.ebp);
	print_asm("leave");
}

make_EHelper(cbw_cwde) {
	int width = g_decoding.is_operand_size_16 ? 1 : 2;
	// sign extend AL -> AX
	rtlreg_t bit;
	rtl_msb(&bit, &cpu.eax, width);
	rtl_andi(&cpu.eax, &cpu.eax, ~(((1U << 8 * width) - 1) << 8 * width) );
	
	rtl_shli(&bit, &bit, width * 8);
	rtl_sub(&cpu.eax, &cpu.eax, &bit);
	
	rtl_shli(&bit, &bit, width * 8);
	rtl_sub(&cpu.eax, &cpu.eax, &bit);
	
	print_asm(g_decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwd_cdq) {
	rtlreg_t bit;
	if(g_decoding.is_operand_size_16) {
		rtl_msb(&bit, &cpu.eax, 2);
		rtl_andi(&cpu.edx, &cpu.edx, 0xFFFF);
		rtl_sub(&cpu.edx, &cpu.edx, &bit);
		rtl_shli(&bit, &bit, 16);
		rtl_sub(&cpu.edx, &cpu.edx, &bit);
		
	} else {
		rtl_msb(&bit, &cpu.eax, 4);
		rtl_andi(&cpu.edx, &cpu.edx, 0);
		rtl_sub(&cpu.edx, &cpu.edx, &bit);
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
    int offset = (4 - id_src->width)*8;
    // fprintf(stderr, "[wtf src = %d, dest = %d, flg=%d, offset=%d]", id_src->width, id_dest->width, g_decoding.is_operand_size_16, offset);
    rtlreg_t tmp, res;
    rtl_shli(&tmp, &id_src->val, offset);
    rtl_shri(&res, &tmp, offset);
    // fprintf(stderr, "{%08x}->[%08x]\n", id_src->val, res);
	operand_write(id_dest, &res);
	print_asm_template2(movzx);
}

make_EHelper(lea) {
	assert(id_src->type == OP_TYPE_MEM);
	operand_write(id_dest, &id_src->addr);
	print_asm_template2(lea);
}
