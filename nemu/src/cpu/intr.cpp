#include <nemu.h>
#include "cpu/exec.h"
#include "memory/mmu.h"
extern void info_register();

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */
	fprintf(stderr, "<<<%08x-%08x|", ret_addr, cpu.esp);
	assert(0 <= NO);
	assert(NO * 8U + 7U < cpu.idtr.limit);
	rtl_push(&cpu.eflags);
	rtl_andi(&cpu.eflags, &cpu.eflags, ~EFLAGS::MASK_IF);
	assert(cpu.cs == 8);
	rtl_push(&cpu.cs);
	rtlreg_t seqEIP;
	rtl_li(&seqEIP, ret_addr);
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
//	fprintf(stderr, "%08x", tempCS);
	fprintf(stderr, "|%08x-%08x>>>", tempEIP, cpu.esp);
	assert(tempCS == 8);
	rtl_jr(&tempEIP);
}

extern uint32_t get_time_in_ms();
bool dev_raise_intr() {
	using namespace EFLAGS;
	static int last_IF = 0;
	int this_IF = get_IF(cpu.eflags);
	static uint32_t last_time = 100000;
	if(last_IF && this_IF && cpu.irq_time == 1){
		// TODO: JIT
//		Log("trigger interrupt at %d=> %08x", last_time, cpu.eip);
		raise_intr(32, cpu.eip);
		cpu.irq_time = 0;
		return true;
	}
	last_IF = this_IF;
	uint32_t new_time = g_nr_guest_instr;
	if(last_time < new_time){
		cpu.irq_time = 1;
		last_time = new_time + 10;
//		extern void info_register();
//		info_register();
	}
	return false;
}

