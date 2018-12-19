#include "cpu/exec.h"

static uint32_t get_min(int width){
	uint32_t min = 0x80000000;
	return min >> (8 * (4 - width));
}

make_EHelper(add) {
	rtlreg_t res, CF, OF, t_xor1, t_xor2, t_and;
	rtl_add(&res, &id_dest->val, &id_src->val);
	rtl_update_ZFSF(&res);
	
	rtl_setrelop(RELOP_LTU, &CF, &res, &id_src->val);
	rtl_update_bit_CF(&CF);
	
	rtl_xor(&t_xor1, &res, &id_dest->val);
	rtl_xor(&t_xor2, &res, &id_src->val);
	rtl_and(&t_and, &t_xor1, &t_xor2);
	rtl_msb(&OF, &t_and, id_dest->width);
	rtl_update_bit_OF(&OF);
	
	operand_write(id_dest, &res);
	
	print_asm_template2(add);
}

static inline void sub_cmp_common_ref(rtlreg_t &res) {
	rtlreg_t CF, OF, ord, sign;
	rtl_sub(&res, &id_dest->val, &id_src->val);
	rtl_update_ZFSF(&res);
	
	rtl_setrelop(RELOP_LTU, &CF, &id_dest->val, &id_src->val);
	rtl_update_bit_CF(&CF);
	
	rtl_setrelop(RELOP_LT, &ord, &id_dest->val, &id_src->val);
	rtl_msb(&sign, &res, id_dest->width);
	rtl_xor(&OF, &ord, &sign);
	rtl_update_bit_OF(&OF);
}

make_EHelper(sub) {
	rtlreg_t res;
	sub_cmp_common_ref(res);
	operand_write(id_dest, &res);
	print_asm_template2(sub);
}

make_EHelper(cmp) {
	rtlreg_t res;
	sub_cmp_common_ref(res);
	print_asm_template2(cmp);
}

make_EHelper(inc) {
//    TODO();
	rtlreg_t res;
	rtlreg_t OF;
	rtl_addi(&res, &id_dest->val, 1);
	rtl_update_ZFSF(&res);
	rtl_setrelopi(RELOP_EQ, &OF, &res, get_min(rtl_width));
	rtl_update_bit_OF(&OF);
	// keep CF
	operand_write(id_dest, &res);
	print_asm_template1(inc);
}

make_EHelper(dec) {
	rtlreg_t res;
	rtlreg_t OF;
	rtl_subi(&res, &id_dest->val, 1);
	rtl_update_ZFSF(&res);
	rtl_setrelopi(RELOP_EQ, &OF, &id_dest->val, get_min(rtl_width));
	rtl_update_bit_OF(&OF);
	// keep CF
	operand_write(id_dest, &res);
	print_asm_template1(dec);
}

make_EHelper(neg) {
    TODO();
	
	print_asm_template1(neg);
}

void rtl_flag_set(const rtlreg_t &res, const rtlreg_t &src1, const rtlreg_t &src2, const rtlreg_t &carry) {
	rtlreg_t CF, OF, t_xor1, t_xor2, t_and, isZero, ord;
	rtl_setrelop(RELOP_LEU, &ord, &res, &src2);
	rtl_setrelopi(RELOP_NE, &isZero, &src1, 0);
	rtl_or(&isZero, &isZero, &carry);
	rtl_and(&CF, &ord, &isZero);
	rtl_update_bit_CF(&CF);
	
	rtl_xor(&t_xor1, &res, &src1);
	rtl_xor(&t_xor2, &res, &src2);
	rtl_and(&t_and, &t_xor1, &t_xor2);
	rtl_msb(&OF, &t_and, id_dest->width);
	rtl_update_bit_OF(&OF);
}

make_EHelper(adc) {
	rtlreg_t res, oldCF;
	rtl_add(&res, &id_dest->val, &id_src->val);
	rtl_get_CF(&oldCF);

	rtl_add(&res, &res, &oldCF);
	rtl_update_ZFSF(&res);
	rtl_flag_set(res, id_dest->val, id_src->val, oldCF);
	
	operand_write(id_dest, &res);
	
	
	print_asm_template2(adc);
}

make_EHelper(sbb) {
	rtlreg_t res, oldCF;
	rtl_sub(&res, &id_dest->val, &id_src->val);
	rtl_get_CF(&oldCF);
	
	rtl_sub(&res, &res, &oldCF);
	rtl_update_ZFSF(&res);
	rtl_flag_set(id_dest->val, id_src->val, res, oldCF);

    operand_write(id_dest, &res);

	print_asm_template2(sbb);
}

make_EHelper(mul) {
	using namespace EFLAGS;
	g_ignore_eflags = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
	rtl_lr(&t0, R_EAX, id_dest->width);
	rtl_mul_lo(&t1, &id_dest->val, &t0);
	
	switch (id_dest->width) {
		case 1:
			rtl_sr(R_AX, &t1, 2);
			break;
		case 2:
			rtl_sr(R_AX, &t1, 2);
			rtl_shri(&t1, &t1, 16);
			rtl_sr(R_DX, &t1, 2);
			break;
		case 4:
			rtl_mul_hi(&t2, &id_dest->val, &t0);
			rtl_sr(R_EDX, &t2, 4);
			rtl_sr(R_EAX, &t1, 4);
			break;
		default:
			assert(0);
	}
	
	print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
	using namespace EFLAGS;
	g_ignore_eflags = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
	rtl_lr(&t0, R_EAX, id_dest->width);
	rtl_imul_lo(&t1, &id_dest->val, &t0);
	
	switch (id_dest->width) {
		case 1:
			rtl_sr(R_AX, &t1, 2);
			break;
		case 2:
			rtl_sr(R_AX, &t1, 2);
			rtl_shri(&t1, &t1, 16);
			rtl_sr(R_DX, &t1, 2);
			break;
		case 4:
			rtl_imul_hi(&t2, &id_dest->val, &t0);
			rtl_sr(R_EDX, &t2, 4);
			rtl_sr(R_EAX, &t1, 4);
			break;
		default:
			assert(0);
	}
	
	print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
	using namespace EFLAGS;
	g_ignore_eflags = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
	rtl_sext(&t0, &id_src->val, id_src->width);
	rtl_sext(&t1, &id_dest->val, id_dest->width);
	
	rtl_imul_lo(&t2, &t1, &t0);
	operand_write(id_dest, &t2);
	
	print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
	using namespace EFLAGS;
	g_ignore_eflags = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
	rtl_sext(&t0, &id_src->val, id_src->width);
	rtl_sext(&t1, &id_src2->val, id_src->width);
	rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
	
	rtl_imul_lo(&t2, &t1, &t0);
	operand_write(id_dest, &t2);
	
	print_asm_template3(imul);
}

make_EHelper(div) {
	using namespace EFLAGS;
	g_ignore_eflags = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
	switch (id_dest->width) {
		case 1:
			rtl_lr(&t0, R_AX, 2);
			rtl_div_q(&t2, &t0, &id_dest->val);
			rtl_div_r(&t3, &t0, &id_dest->val);
			rtl_sr(R_AL, &t2, 1);
			rtl_sr(R_AH, &t3, 1);
			break;
		case 2:
			rtl_lr(&t0, R_AX, 2);
			rtl_lr(&t1, R_DX, 2);
			rtl_shli(&t1, &t1, 16);
			rtl_or(&t0, &t0, &t1);
			rtl_div_q(&t2, &t0, &id_dest->val);
			rtl_div_r(&t3, &t0, &id_dest->val);
			rtl_sr(R_AX, &t2, 2);
			rtl_sr(R_DX, &t3, 2);
			break;
		case 4:
			rtl_lr(&t0, R_EAX, 4);
			rtl_lr(&t1, R_EDX, 4);
			rtl_div64_q(&cpu.eax, &t1, &t0, &id_dest->val);
			rtl_div64_r(&cpu.edx, &t1, &t0, &id_dest->val);
			break;
		default:
			assert(0);
	}
	
	print_asm_template1(div);
}

make_EHelper(idiv) {
	using namespace EFLAGS;
	g_ignore_eflags = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
	switch (id_dest->width) {
		case 1:
			rtl_lr(&t0, R_AX, 2);
			rtl_idiv_q(&t2, &t0, &id_dest->val);
			rtl_idiv_r(&t3, &t0, &id_dest->val);
			rtl_sr(R_AL, &t2, 1);
			rtl_sr(R_AH, &t3, 1);
			break;
		case 2:
			rtl_lr(&t0, R_AX, 2);
			rtl_lr(&t1, R_DX, 2);
			rtl_shli(&t1, &t1, 16);
			rtl_or(&t0, &t0, &t1);
			rtl_idiv_q(&t2, &t0, &id_dest->val);
			rtl_idiv_r(&t3, &t0, &id_dest->val);
			rtl_sr(R_AX, &t2, 2);
			rtl_sr(R_DX, &t3, 2);
			break;
		case 4:
			rtl_lr(&t0, R_EAX, 4);
			rtl_lr(&t1, R_EDX, 4);
			rtl_idiv64_q(&cpu.eax, &t1, &t0, &id_dest->val);
			rtl_idiv64_r(&cpu.edx, &t1, &t0, &id_dest->val);
			break;
		default:
			assert(0);
	}
	
	print_asm_template1(idiv);
}
