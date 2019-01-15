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





// void jit_rtl_host_lm(rtlreg_t *dest, const void *addr, int len) {
// 	JIT_TODO;
// 	switch (len) {
// 		case 4:
// 			*dest = *(uint32_t *) addr;
// 			return;
// 		case 1:
// 			*dest = *(uint8_t *) addr;
// 			return;
// 		case 2:
// 			*dest = *(uint16_t *) addr;
// 			return;
// 		default:
// 			panic("wtf");
// 	}
// }

// void jit_rtl_host_sm(void *addr, const rtlreg_t *src1, int len) {
// 	JIT_TODO;
// 	switch (len) {
// 		case 4:
// 			*(uint32_t *) addr = *src1;
// 			return;
// 		case 1:
// 			*(uint8_t *) addr = (uint8_t)*src1;
// 			return;
// 		case 2:
// 			*(uint16_t *) addr = (uint16_t) *src1;
// 			return;
// 		default:
// 			panic("wtf");
// 	}
// }

void jit_rtl_setrelop(uint32_t relop, rtlreg_t *dest, const rtlreg_t *src1_raw,
		const rtlreg_t *src2_raw) {
	JIT_DONE;
	uint32_t offset = (4 - rtl_width) * 8;
	uint32_t src1 = *src1_raw << offset;
	uint32_t src2 = *src2_raw << offset;
	
#if JIT_COMPILE_FLAG
	auto va_raw = eng.get_value(src1_raw);
	auto vb_raw = eng.get_value(src2_raw);
	auto va = eng().CreateShl(va_raw, offset);
	auto vb = eng().CreateShl(vb_raw, offset);
#else
	llvm::Value* va;
	llvm::Value* vb;
#endif
	
	llvm::Value* vres;
	switch (relop) {
		case RELOP_FALSE: {
			*dest = (uint32_t)(false);
			JIT_COMPILE_BARRIER;
			vres = eng().getInt1(false);
			break;
		}
		case RELOP_TRUE: {
			*dest = (uint32_t)(true);
			JIT_COMPILE_BARRIER;
			vres = eng().getInt1(true);
			break;
		}
		case RELOP_EQ: {
			*dest = (uint32_t)(src1 == src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpEQ(va, vb);
			break;
		}
		case RELOP_NE: {
			*dest = (uint32_t)(src1 != src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpNE(va, vb);
			break;
		}
		case RELOP_LT: {
			*dest = (uint32_t)((int32_t) src1 < (int32_t) src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpSLT(va, vb);
			break;
		}
		case RELOP_LE: {
			*dest = (uint32_t)((int32_t) src1 <= (int32_t) src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpSLE(va, vb);
			break;
		}
		case RELOP_GT: {
			*dest = (uint32_t)((int32_t) src1 > (int32_t) src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpSGT(va, vb);
			break;
		}
		case RELOP_GE: {
			*dest = (uint32_t)((int32_t) src1 >= (int32_t) src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpSGE(va, vb);
			break;
		}
		case RELOP_LTU: {
			*dest = (uint32_t)(src1 < src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpULT(va, vb);
			break;
		}
		case RELOP_LEU: {
			*dest = (uint32_t)(src1 <= src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpULE(va, vb);
			break;
		}
		case RELOP_GTU: {
			*dest = (uint32_t)(src1 > src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpUGT(va, vb);
			break;
		}
		case RELOP_GEU: {
			*dest = (uint32_t)(src1 >= src2);
			JIT_COMPILE_BARRIER;
			vres = eng().CreateICmpUGE(va, vb);
			break;
		}
		default:
			panic("unsupport relop = %d", relop);
	}
	auto vres_int = eng().CreateIntCast(vres, eng.getRegTy(), false);
	eng.set_value(dest, vres_int);
}

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

template<int n>
bool is_align(uint32_t addr){
    return ((n - 1) & (uint32_t)addr) == 0;
}

llvm::Value* blender(llvm::Value* vbase, llvm::Value* val_vaddr, int shift, uint32_t off_mask) {
	auto vbase_offset = eng().CreateAnd(vbase, ~maskify(12));
	auto vjam_offset = eng().CreateLShr(val_vaddr, shift);
	auto vreal_offset = eng().CreateAnd(vjam_offset, off_mask);
	return eng().CreateAdd(vbase_offset, vreal_offset);
}

llvm::Value* ptr_trans(int len, llvm::Value* vptr_raw){
	llvm::Type* type;
	switch(len){
		case 4: type = eng().getInt32Ty(); break;
		case 2: type = eng().getInt16Ty(); break;
		case 1: type = eng().getInt8Ty(); break;
		default:
			panic("wtf");
	}
	auto vptr = eng().CreateBitOrPointerCast(vptr_raw, type->getPointerTo());
	return vptr;
}

llvm::Value* mem_gep(int len, llvm::Value* vpaddr){
	auto vpmem = eng().CreateIntToPtr(eng().getInt64((uint64_t)pmem), eng().getInt8Ty()->getPointerTo());
	auto vptr_raw = eng().CreateGEP(vpmem, vpaddr);
	auto vptr = ptr_trans(len, vptr_raw);
	return vptr;
}

llvm::Value* extract_vptr(int len, llvm::Value* vbase, llvm::Value* val_vaddr, int shift, uint32_t off_mask) {
	auto vpaddr = blender(vbase, val_vaddr, shift, off_mask);
	auto vptr = mem_gep(len, vpaddr);
	return vptr;
}

llvm::Value* fetch_entry(llvm::Value* vbase, llvm::Value* val_vaddr, int shift, uint32_t off_mask){
	using namespace llvm;
	auto vptr = extract_vptr(4, vbase, val_vaddr, shift, off_mask);
	return eng().CreateLoad(vptr);
}


#include "device/mmio.h"
void jit_rtl_lm(rtlreg_t* dest, const rtlreg_t *mem_addr, int width){
	JIT_DONE;
	int NO = is_mmio(*mem_addr);
	if(NO >= 0){
		*dest = mmio_read(*mem_addr, width, NO);
#if JIT_COMPILE_FLAG
		JIT_TODO;
		using namespace llvm;
		auto FT = FunctionType::get(eng.getRegTy(), {eng.getIntTy(), eng.getIntTy(), eng.getIntTy()}, false);
		auto FAddr = eng().getInt64((uint64_t)mmio_read);
		auto F = eng().CreateIntToPtr(FAddr, FT->getPointerTo());
		
		auto vmem_addr = eng.get_value(mem_addr);
		auto vwidth = eng().getInt32(width);
		auto vno = eng().getInt32(NO);
		auto vres = eng().CreateCall(F, {vmem_addr, vwidth, vno});
		eng.set_value(dest, vres);
#endif
		return;
	}
	uint32_t paddr;
	llvm::Value* vpaddr = nullptr;
	if(!cpu.cr0.paging){
		paddr = *mem_addr;
#if JIT_COMPILE_FLAG
		vpaddr = eng.get_value(mem_addr);
#endif
	} else {
		JIT_TODO;
		// 0 12 22 32
		uint32_t vaddr = *mem_addr;
		uint32_t pde = ((uint32_t*)(pmem + (cpu.cr3.val & ~maskify(12))))[vaddr >> 22];
		uint32_t pte = ((uint32_t*)(pmem + (pde & ~maskify(12)))) [(vaddr >> 12) & maskify(10)];
		paddr = (pte & ~maskify(12)) + (vaddr & maskify(12));
		// keep it functional
#if JIT_COMPILE_FLAG
		// the hardest part
#endif
	}
	
    void* ptr = pmem + paddr;
	switch (width){
		case 4: *dest = *(uint32_t*)ptr; assert(is_align<4>(paddr)); break;
		case 2: *dest = *(uint16_t*)ptr; assert(is_align<2>(paddr)); break;
		case 1: *dest = *(uint8_t*)ptr; break;
	}
	JIT_COMPILE_BARRIER;
	auto vptr = mem_gep(width, vpaddr);
	auto vdata_raw = eng().CreateLoad(vptr);
	auto vdata = eng().CreateIntCast(vdata_raw, eng.getRegTy(), false);
	eng.set_value(dest, vdata);
}

void jit_rtl_lm_back(rtlreg_t *dest, const rtlreg_t *vaddr, int len) {
	JIT_DONE;
	*dest = vaddr_read(*vaddr, len);
	
	JIT_COMPILE_BARRIER;
	using namespace llvm;
	
	auto FT = FunctionType::get(eng.getRegTy(), {eng.getRegTy(), eng.getIntTy()}, false);
	auto FAddr = eng().getInt64((uint64_t)vaddr_read);
	auto F = eng().CreateIntToPtr(FAddr, FT->getPointerTo());
	
	auto val_vaddr = eng.get_value(vaddr);
	auto val_len = eng().getInt32(len);
	auto val_ret = eng().CreateCall(F, {val_vaddr, val_len});
	eng.set_value(dest, val_ret);
	
}

void jit_rtl_sm(const rtlreg_t *vaddr, const rtlreg_t *src1, int len) {
	JIT_DONE;
	vaddr_write(*vaddr, *src1, len);
	
	JIT_COMPILE_BARRIER;
	using namespace llvm;
	auto FT = FunctionType::get(eng().getVoidTy(), {eng.getRegTy(), eng.getRegTy(), eng.getIntTy()}, false);
	auto FAddr = eng().getInt64((uint64_t)vaddr_write);
	auto F = eng().CreateIntToPtr(FAddr, FT->getPointerTo());
	
	auto val_vaddr = eng.get_value(vaddr);
	auto val_src = eng.get_value(src1);
	auto val_len = eng().getInt32(len);
	eng().CreateCall(F, {val_vaddr, val_src, val_len});
}
