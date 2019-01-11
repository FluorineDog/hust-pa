#include "cpu/jit.h"
#include "workflow.h"
#include "cpu/exec.h"

#define JIT_HEADER do{ printf("%s\n", __FUNCTION__); }while(0)
#define JIT_TODO do{printf("todo: ");JIT_HEADER; assert(0 + 1);}while(0)
#define JIT_DONE JIT_HEADER
#define JIT_COMPILE return

llvm::CodeExecutor eng;
namespace jit {

enum class JITState {
	Init,
	Compiling,
	Terminate,
};
JITState state_ = JITState::Init;

inline void decoding_set_jmp(bool is_jmp) {
	assert(is_jmp == true);
	assert(state_ == JITState::Compiling);
	state_ = JITState::Terminate;
	g_decoding.is_jmp = is_jmp;
}


std::optional<int> exec_or_open(vaddr_t cr3, vaddr_t eip) {
	switch (state_) {
		case JITState::Init: {
			if (auto query = eng.fetchFunction(cr3, eip)) {
				auto[func, expected_inst] = query.value();
				auto real_inst = func((uint32_t *) &cpu, nullptr);
				assert(expected_inst == real_inst);
				(void) real_inst;
				return expected_inst;
			}
//			eng.begin_block(cr3, eip);
			state_ = JITState::Compiling;
			break;
		}
		case JITState::Compiling: {
			break;
		}
		case JITState::Terminate: {
			assert(0);
		}
	}
	
	assert(state_ == JITState::Compiling);
	return std::nullopt;
}

using namespace jit;

void exec_close() {
	if (state_ == JITState::Terminate) {
		printf("-------------\n");
		state_ = JITState::Init;
//		eng.finish_block();
	}
}

void inst_barrier() {
	JIT_COMPILE;
//	eng.finish_inst();
}


}    //namespace jit

void jit_rtl_li(rtlreg_t *dest, uint32_t imm) {
	JIT_DONE;
	*dest = imm;
	
	JIT_COMPILE;
	auto vimm = eng().getInt32(imm);
	eng.set_value(dest, vimm);
}

void jit_rtl_mv(rtlreg_t *dest, const rtlreg_t *src1) {
	JIT_DONE;
	*dest = *src1;
	
	JIT_COMPILE;
	auto val = eng.get_value(src1);
	eng.set_value(dest, val);
}

//make_rtl_arith_logic_dog(add)
void jit_rtl_add(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 + *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateAdd(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(sub)
void jit_rtl_sub(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 - *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateSub(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(and)
void jit_rtl_and(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 & *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateAnd(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(or)
void jit_rtl_or(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 | *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateOr(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(xor)
void jit_rtl_xor(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 ^ *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateXor(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(shl)
void jit_rtl_shl(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 << *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateShl(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(shr)
void jit_rtl_shr(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 >> *src2;
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateLShr(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(sar)
void jit_rtl_sar(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = (uint32_t) ((int32_t) *src1 >> *src2);
	
	JIT_COMPILE;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateAShr(va, vb);
	eng.set_value(dest, vres);
}

#define make_rtl_arith_logic_dog(name)                                                \
    void name_concat3(RTL_PREFIX, _rtl_, name)(rtlreg_t * dest, const rtlreg_t *src1, \
                                               const rtlreg_t *src2) {                \
        JIT_TODO;                                                                \
        *dest = name_concat(c_, name)(*src1, *src2);                                  \
    }

make_rtl_arith_logic_dog(mul_lo);

make_rtl_arith_logic_dog(mul_hi);

make_rtl_arith_logic_dog(imul_lo);

make_rtl_arith_logic_dog(imul_hi);

make_rtl_arith_logic_dog(div_q);

make_rtl_arith_logic_dog(div_r);

make_rtl_arith_logic_dog(idiv_q);

make_rtl_arith_logic_dog(idiv_r);

void jit_rtl_div64_q(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2) {
	JIT_TODO;
	uint64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = (uint32_t) (dividend / divisor);
}

void jit_rtl_div64_r(rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo,
		const rtlreg_t *src2) {
	JIT_TODO;
	uint64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = (uint32_t) (dividend % divisor);
}

void jit_rtl_idiv64_q(rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo,
		const rtlreg_t *src2) {
	JIT_TODO;
	int64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = (uint32_t) (dividend / divisor);
}

void jit_rtl_idiv64_r(rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo,
		const rtlreg_t *src2) {
	JIT_TODO;
	int64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = (uint32_t) (dividend % divisor);
}

void jit_rtl_lm(rtlreg_t *dest, const rtlreg_t *addr, int len) {
	JIT_TODO;
	*dest = vaddr_read(*addr, len);
}

void jit_rtl_sm(const rtlreg_t *addr, const rtlreg_t *src1, int len) {
	JIT_TODO;
	vaddr_write(*addr, *src1, len);
}

void jit_rtl_host_lm(rtlreg_t *dest, const void *addr, int len) {
	JIT_TODO;
	switch (len) {
		case 4:
			*dest = *(uint32_t *) addr;
			return;
		case 1:
			*dest = *(uint8_t *) addr;
			return;
		case 2:
			*dest = *(uint16_t *) addr;
			return;
		default:
			panic("wtf");
	}
}

void jit_rtl_host_sm(void *addr, const rtlreg_t *src1, int len) {
	JIT_TODO;
	switch (len) {
		case 4:
			*(uint32_t *) addr = *src1;
			return;
		case 1:
			*(uint8_t *) addr = *src1;
			return;
		case 2:
			*(uint16_t *) addr = *src1;
			return;
		default:
			panic("wtf");
	}
}

void jit_rtl_setrelop(uint32_t relop, rtlreg_t *dest, const rtlreg_t *src1_raw,
		const rtlreg_t *src2_raw) {
	JIT_TODO;
	int offset = (4 - rtl_width) * 8;
	uint32_t src1 = *src1_raw << offset;
	uint32_t src2 = *src2_raw << offset;
	switch (relop) {
		case RELOP_FALSE: {
			*dest = false;
			break;
		}
		case RELOP_TRUE: {
			*dest = true;
			break;
		}
		case RELOP_EQ: {
			*dest = src1 == src2;
			break;
		}
		case RELOP_NE: {
			*dest = src1 != src2;
			break;
		}
		case RELOP_LT: {
			*dest = (int32_t) src1 < (int32_t) src2;
			break;
		}
		case RELOP_LE: {
			*dest = (int32_t) src1 <= (int32_t) src2;
			break;
		}
		case RELOP_GT: {
			*dest = (int32_t) src1 > (int32_t) src2;
			break;
		}
		case RELOP_GE: {
			*dest = (int32_t) src1 >= (int32_t) src2;
			break;
		}
		case RELOP_LTU: {
			*dest = src1 < src2;
			break;
		}
		case RELOP_LEU: {
			*dest = src1 <= src2;
			break;
		}
		case RELOP_GTU: {
			*dest = src1 > src2;
			break;
		}
		case RELOP_GEU: {
			*dest = src1 >= src2;
			break;
		}
		default:
			panic("unsupport relop = %d", relop);
	}
//	*dest = internal_relop(relop, *src1, *src2, rtl_width);
}

void jit_rtl_j(vaddr_t target) {
	JIT_TODO;
	cpu.eip = target;
	jit::decoding_set_jmp(true);
}

void jit_rtl_jr(rtlreg_t *target) {
	JIT_TODO;
	// very hard to use
	cpu.eip = *target;
	
	jit::decoding_set_jmp(true);
}

void jit_rtl_jcond(const rtlreg_t *cond, vaddr_t target) {
	JIT_DONE;
	auto is_jmp = *cond;
	if (is_jmp)
		cpu.eip = target;
	else
		cpu.eip = g_decoding.seq_eip;
	jit::decoding_set_jmp(true);
	
	JIT_COMPILE;
	auto vtarget = eng().getInt32(target);
	auto vseq = eng().getInt32(g_decoding.seq_eip);
	auto vcond = eng.get_value(cond);
	auto vjmp = eng().CreateSelect(vcond, vtarget, vseq);
	eng.set_value(&cpu.eip, vjmp);
}

void jit_rtl_exit(int state);