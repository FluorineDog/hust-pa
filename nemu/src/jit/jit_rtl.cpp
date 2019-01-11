#include "cpu/jit.h"
#include "workflow.h"
#include "cpu/exec.h"




namespace jit{

enum class JITState {
	Init,
	Compiling,
	Terminate,
};

static llvm::CodeExecutor eng;
static JITState state_ = JITState::Init;


std::optional<int> exec_exec_or_open(vaddr_t cr3, vaddr_t eip) {
	switch (state_) {
		case JITState::Init: {
			if (auto query = eng.fetchFunction(cr3, eip)) {
				auto[func, expected_inst]  = query.value();
				auto real_inst = func((uint32_t *) &cpu, nullptr);
				assert(expected_inst == real_inst);
                (void)real_inst;
				return expected_inst;
			}
			eng.begin_block(cr3, eip);
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

void exec_close() {
	eng.finish_inst();

	if (state_ == JITState::Terminate) {
		state_ = JITState::Init;
		eng.finish_block();
	}
}

void inst_barrier() {

}
} //namespace jit

void jit_rtl_li(rtlreg_t *dest, uint32_t imm) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = imm;
}

void jit_rtl_mv(rtlreg_t *dest, const rtlreg_t *src1) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1;
}


                                               \


//make_rtl_arith_logic_dog(add)
void jit_rtl_add(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 + *src2;
}
//make_rtl_arith_logic_dog(sub)
void jit_rtl_sub(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 - *src2;
}
//make_rtl_arith_logic_dog(and)
void jit_rtl_and(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 & *src2;
}
//make_rtl_arith_logic_dog(or)
void jit_rtl_or(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 | *src2;
}

//make_rtl_arith_logic_dog(xor)
void jit_rtl_xor(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 ^ *src2;
}
//make_rtl_arith_logic_dog(shl)
void jit_rtl_shl(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 << *src2;
}
//make_rtl_arith_logic_dog(shr)
void jit_rtl_shr(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = *src1 >> *src2;
}
//make_rtl_arith_logic_dog(sar)
void jit_rtl_sar(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2){
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = (uint32_t)((int32_t)*src1 >> *src2);
}

#define make_rtl_arith_logic_dog(name)                          \
void name_concat3(RTL_PREFIX, _rtl_, name)(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2) {              \
    assert(0 + 0); \
    *dest = name_concat(c_, name)(*src1, *src2);                                \
}                                

make_rtl_arith_logic_dog(mul_lo)
make_rtl_arith_logic_dog(mul_hi)
make_rtl_arith_logic_dog(imul_lo)
make_rtl_arith_logic_dog(imul_hi)
make_rtl_arith_logic_dog(div_q)
make_rtl_arith_logic_dog(div_r)
make_rtl_arith_logic_dog(idiv_q)
make_rtl_arith_logic_dog(idiv_r)

void jit_rtl_div64_q(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	uint64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = dividend / divisor;
}
void jit_rtl_div64_r(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	uint64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = dividend % divisor;
}

void jit_rtl_idiv64_q(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	int64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = dividend / divisor;
}

void jit_rtl_idiv64_r(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	int64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = dividend % divisor;
}

void jit_rtl_lm(rtlreg_t *dest, const rtlreg_t *addr, int len) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = vaddr_read(*addr, len);
}

void jit_rtl_sm(const rtlreg_t *addr, const rtlreg_t *src1, int len) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	vaddr_write(*addr, *src1, len);
}

void jit_rtl_host_lm(rtlreg_t *dest, const void *addr, int len) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
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
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
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

void jit_rtl_setrelop(uint32_t relop, rtlreg_t *dest,
		const rtlreg_t *src1, const rtlreg_t *src2) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	*dest = internal_relop(relop, *src1, *src2, rtl_width);
}

void jit_rtl_j(vaddr_t target) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	cpu.eip = target;
	decoding_set_jmp(true);
}

void jit_rtl_jr(rtlreg_t *target) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	// very hard to use
	cpu.eip = *target;
	decoding_set_jmp(true);
}

void jit_rtl_jcond(const rtlreg_t *cond, vaddr_t target) {
	printf("%s\n", __FUNCTION__); //assert(0 + 0);
	bool is_jmp = *cond;
	if (is_jmp) cpu.eip = target;
    else cpu.eip = g_decoding.seq_eip;
	decoding_set_jmp(true);
}

void jit_rtl_exit(int state);