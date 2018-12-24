#include <nemu.h>
#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */
	assert(NO * 8 + 7 < cpu.idtr.limit);
	rtl_push()
	rtlreg_t addr;
	rtl_addi(&addr, &cpu.idtr.base, NO * 8);
	
	TODO();
}

void dev_raise_intr() {
}

