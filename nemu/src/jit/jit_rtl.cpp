#include "jit_rtl.h"


llvm::CodeExecutor eng;

namespace jit{

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
                // assert(cr3 == 0);
				auto[func, expected_inst] = query.value();
#ifdef DEBUG
                fprintf(stderr, "exec block %x with %d insts\n", eip, expected_inst);
#endif
				auto real_inst = func((uint32_t *) &cpu, nullptr);
				assert(expected_inst == real_inst);
				(void) real_inst;
				return expected_inst;
			}
#if JIT_COMPILE_FLAG
			eng.begin_block(cr3, eip);
#endif
			
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
#if JIT_COMPILE_FLAG
//		printf("-------------\n");
#endif
		state_ = JITState::Init;
#if JIT_COMPILE_FLAG
		eng.finish_block();
#endif
	}
}

void inst_barrier() {
	JIT_COMPILE_BARRIER;
#if JIT_COMPILE_FLAG
	eng.finish_inst();
#endif
}


}    //namespace jit




void jit_rtl_j(vaddr_t target) {
	JIT_DONE;
	cpu.eip = target;
	jit::decoding_set_jmp(true);
	
	JIT_COMPILE_BARRIER;
	auto vtarget = eng().getInt32(target);
	eng.set_value(&cpu.eip, vtarget);
}

void jit_rtl_jr(rtlreg_t *target) {
	JIT_DONE;
	// very hard to use
	cpu.eip = *target;
	jit::decoding_set_jmp(true);

	JIT_COMPILE_BARRIER;
	auto vtar = eng.get_value(target);
	eng.set_value(&cpu.eip, vtar);
}

void jit_rtl_jcond(const rtlreg_t *cond, vaddr_t target) {
	JIT_DONE;
	auto is_jmp = *cond;
	if (is_jmp)
		cpu.eip = target;
	else
		cpu.eip = g_decoding.seq_eip;
	jit::decoding_set_jmp(true);
	
	JIT_COMPILE_BARRIER;
	auto vtarget = eng().getInt32(target);
	auto vseq = eng().getInt32(g_decoding.seq_eip);
	auto vcond_int = eng.get_value(cond);
	auto v0 = eng().getInt32(0);
	auto vcond = eng().CreateICmpNE(vcond_int, v0);
	auto vjmp = eng().CreateSelect(vcond, vtarget, vseq);
	eng.set_value(&cpu.eip, vjmp);
}

void jit_rtl_active(rtlreg_t *dest){
	JIT_DONE;
	// active uninitilized variable, suppress reg check assert
	
	JIT_COMPILE_BARRIER;
	eng.active_value(dest);
}


#include "device/port-io.h"
void jit_rtl_io_in(rtlreg_t* dest, const rtlreg_t* ioaddr, int width){
	using namespace llvm;
	JIT_DONE;
	*dest = pio_read_common(*ioaddr, width);
	
	JIT_COMPILE_BARRIER;
	auto FT = FunctionType::get(eng.getRegTy(), {eng.getRegTy(), eng.getIntTy()}, false);
	auto FAddr = eng().getInt64((uint64_t)pio_read_common);
	auto F = eng().CreateIntToPtr(FAddr, FT->getPointerTo());
	
	auto vioaddr = eng.get_value(ioaddr);
	auto vwidth = eng().getInt32(width);
	auto vdata = eng().CreateCall(F, {vioaddr, vwidth});
	eng.set_value(dest, vdata);
}

void jit_rtl_io_out(const rtlreg_t* ioaddr, const rtlreg_t* src, int width){
	using namespace llvm;
	JIT_DONE;
	pio_write_common(*ioaddr, *src,  width);
	
	
	JIT_COMPILE_BARRIER;
	auto FT = FunctionType::get(eng().getVoidTy(), {eng.getRegTy(), eng.getRegTy(), eng.getIntTy()}, false);
	auto FAddr = eng().getInt64((uint64_t)pio_write_common);
	auto F = eng().CreateIntToPtr(FAddr, FT->getPointerTo());
	
	auto vioaddr = eng.get_value(ioaddr);
	auto vsrc = eng.get_value(src);
	auto vwidth = eng().getInt32(width);
	eng().CreateCall(F, {vioaddr, vsrc, vwidth});
	
}

void jit_rtl_exit(int state);


