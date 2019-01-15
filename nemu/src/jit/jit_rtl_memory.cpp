#include "jit_rtl.h"

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