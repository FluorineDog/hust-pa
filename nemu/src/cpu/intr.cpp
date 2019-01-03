#include <nemu.h>
#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */
	assert(0 <= NO);
	assert(NO * 8U + 7U < cpu.idtr.limit);
	rtl_push(&cpu.eflags);
	rtl_push(&cpu.cs);
	rtlreg_t seqEIP;
	rtl_li(&seqEIP, g_decoding.seq_eip);
	rtl_push(&seqEIP);
	rtlreg_t s_addr;
	rtlreg_t gate_0_31, gate_32_63;
	rtl_addi(&s_addr, &cpu.idtr.base, NO * 8);
	
	// load gate
	rtl_lm(&gate_0_31, &s_addr, 4);	
	rtl_addi(&s_addr, &s_addr, 4);
	rtl_lm(&gate_32_63, &s_addr, 4);
	assert( ((gate_32_63 >> 15) & 1)  == 1 );
	// cs == 8
	assert( (gate_0_31 >> 16) == 0x8);
	
	rtlreg_t offset, off_hi, off_lo;
	rtl_li(&off_hi, 0xFFFF0000);
	rtl_li(&off_lo, 0x0000FFFF);	
	rtl_andi(&off_hi, &off_hi, gate_32_63);
	rtl_andi(&off_lo, &off_lo, gate_0_31);
	rtl_or(&offset, &off_hi, &off_lo);
	
	rtl_jr(&offset);
}

void return_from_intr(){
	rtlreg_t tempEIP, tempCS, tempEFLAGS;
	rtl_pop(&tempEIP);
	rtl_pop(&tempCS);
	rtl_pop(&tempEFLAGS);
	rtl_mv(&cpu.eflags, &tempEFLAGS);
	assert(tempCS == 8);
	rtl_jr(&tempEIP);
}

void dev_raise_intr() {
	// wtf
	
}

