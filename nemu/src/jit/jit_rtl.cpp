#include "cpu/jit.h"
#include "workflow.h"
#include "cpu/exec.h"


enum class JITState {
	Init,
	Compiling,
	Terminate,
};

static llvm::CodeExecutor eng;
static JITState state_ = JITState::Init;


bool jit_exec_exec_or_open(vaddr_t cr3, vaddr_t eip) {
	switch (state_) {
		case JITState::Init: {
			if (auto query = eng.fetchFunction(cr3, eip)) {
				auto[func, expected_inst]  = query.value();
				auto real_inst = func((uint32_t *) &cpu, nullptr);
				assert(expected_inst == real_inst);
				return true;
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
	return false;
}

void jit_exec_close() {
	eng.finish_inst();

	if (state_ == JITState::Terminate) {
		state_ = JITState::Init;
		eng.finish_block();
	}
}

bool jit_exec(vaddr_t *eip) {

	return false;
}

void inst_barrier() {

}

