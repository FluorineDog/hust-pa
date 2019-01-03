#pragma once

#include "common.h"
#include "memory/mmu.h"

enum {
	R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI
};
enum {
	R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI
};
enum {
	R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH
};

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	
	
	/* Do NOT change the order of the GPRs' definitions. */
	
	/* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];
		struct {
			rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	vaddr_t eip;
	rtlreg_t eflags;
	rtlreg_t cs;
	struct {
		rtlreg_t limit;
		rtlreg_t base;
	} idtr;
	union {
		rtlreg_t ctlreg[8];
		struct {
			CR0 cr0;
			rtlreg_t _reserved_1_2[2];
			CR3 cr3;
			rtlreg_t _reserved_4_7[4];
		};
	};
	int irq_time;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char *regsl[];
extern const char *regsw[];
extern const char *regsb[];

// bit-width, gpr_id

std::optional<std::tuple<const char *, size_t>> parse_cpuname(std::string_view str);

static inline const char *reg_name(int index, int width) {
	assert(index >= 0 && index < 8);
	switch (width) {
		case 4:
			return regsl[index];
		case 1:
			return regsb[index];
		case 2:
			return regsw[index];
		default:
			panic("wtf");
	}
}
