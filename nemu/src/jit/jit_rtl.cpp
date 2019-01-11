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
	*dest = imm;
}

void jit_rtl_mv(rtlreg_t *dest, const rtlreg_t *src1) {
	*dest = *src1;
}


#define make_rtl_arith_logic_dog(name)                          \
void name_concat3(RTL_PREFIX, _rtl_, name)(rtlreg_t * dest, const rtlreg_t *src1, const rtlreg_t *src2) {              \
    *dest = name_concat(c_, name)(*src1, *src2);                                \
}                                                                               \

make_rtl_arith_logic_dog(add)
make_rtl_arith_logic_dog(sub)
make_rtl_arith_logic_dog(and)
make_rtl_arith_logic_dog(or)
make_rtl_arith_logic_dog(xor)
make_rtl_arith_logic_dog(shl)
make_rtl_arith_logic_dog(shr)
make_rtl_arith_logic_dog(sar)
make_rtl_arith_logic_dog(mul_lo)
make_rtl_arith_logic_dog(mul_hi)
make_rtl_arith_logic_dog(imul_lo)
make_rtl_arith_logic_dog(imul_hi)
make_rtl_arith_logic_dog(div_q)
make_rtl_arith_logic_dog(div_r)
make_rtl_arith_logic_dog(idiv_q)
make_rtl_arith_logic_dog(idiv_r)