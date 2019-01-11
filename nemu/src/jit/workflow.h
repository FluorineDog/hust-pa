#pragma once

#include <optional>
#include "kjit.h"
#include <map>
#include <memory>
#include <unordered_map>
#include <set>
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>



#include "cpu/reg.h"

namespace llvm {

class CodeExecutor {
private:
	// cpu and memory
	using RawFT = int (*)(uint32_t *, char *);

public:
	CodeExecutor() : builder_(ctx_) {
		InitEnvironment();
		init();
	}
	
	void init() {
		this->jit_ = cantFail(orc::KaleidoscopeJIT::Create());
	}
	
	static void InitEnvironment() {
		LLVMInitializeNativeTarget();
		LLVMInitializeNativeAsmPrinter();
	}
	
	void begin_block(uint32_t cr3, uint32_t vaddr) {
		// clear state;
		s_.clear();
		
		auto uid = get_uid(cr3, vaddr);
		auto name = get_name(uid);
		s_.uid = uid;
		s_.mod = std::make_unique<Module>("mod_" + name, ctx_);
		s_.func = cast<Function>(s_.mod->getOrInsertFunction(name, getFunctorTy()));
		s_.bb = BasicBlock::Create(ctx_, "entry", s_.func);
		builder_.SetInsertPoint(s_.bb);
		assert(s_.func->arg_size() == 2);
		auto iter = s_.func->arg_begin();
		s_.regfile = &*iter;
		++iter;
		s_.memory = &*iter;
		++iter;
		assert(iter == s_.func->arg_end());
	}
	
	LLVMContext &get_ctx() {
		return ctx_;
	}
	
	void set_value(rtlreg_t *reg, Value *value) {
		if (auto reg_id = is_cpu(reg)) {
			int id = reg_id.value();
#if 0
			auto reg_ptr = get_cpu_reg_ptr(id);
			builder_.CreateStore(value, reg_ptr);
#endif
			// dirty now
			s_.reg_cache[id] = std::make_pair(value, true);
		} else {
			s_.value_cache[reg] = value;
		}
	}
	
	Value *get_mem_ptr(uint32_t paddr) {
		return builder_.CreateConstGEP1_32(s_.memory, paddr);
	}
	
	Value *get_value(const rtlreg_t *reg) {
		if (auto reg_id = is_cpu(reg)) {
			int id = reg_id.value();
			if (s_.reg_cache[id].first == nullptr) {
				auto reg_ptr = get_cpu_reg_ptr(id);
				// not dirty yet
				s_.reg_cache[id] = std::make_pair(builder_.CreateLoad(reg_ptr), false);
			}
			return s_.reg_cache[id].first;
		} else {
			assert(s_.value_cache.count(reg));
			return s_.value_cache[reg];
			
		}
	}
	
	void finish_inst() {
		s_.inst_count++;
	}
	
	void finish_block() {
		for (size_t id = 0; id < s_.reg_cache.size(); ++id) {
			auto[value, dirty] = s_.reg_cache[id];
			if (dirty) {
				auto reg_ptr = get_cpu_reg_ptr(id);
				builder_.CreateStore(value, reg_ptr);
			}
		}
		builder_.CreateRet(builder_.getInt32(s_.inst_count));
		outs() << *s_.mod;
		auto err = jit_->addModule(std::move(s_.mod));
		assert(!err);
		auto uid = s_.uid;
		assert(uid != (uint64_t) -1);
		assert(icache.count(uid) == 0);
		auto name = get_name(uid);
		auto symbol = this->jit_->lookup(name);
		icache[uid].first = (RawFT) cantFail(std::move(symbol)).getAddress();
		icache[uid].second = s_.inst_count;
		s_.clear();
	}
	
	std::optional<std::pair<RawFT, int>> fetchFunction(uint32_t cr3, uint32_t vaddr) {
		auto uid = get_uid(cr3, vaddr);
		if (icache.count(uid) == 0) {
			return std::nullopt;
		}
		return icache[uid];
	}
	
	Type *getRegTy() {
		return Type::getInt32Ty(ctx_);
	}
	
	
	FunctionType *getFunctorTy() {
		static FunctionType *functor_type = nullptr;
		if (!functor_type) {
//			auto&& params_type = ;
			functor_type = FunctionType::get(Type::getInt32Ty(ctx_),
					{getRegTy()->getPointerTo(), Type::getInt8PtrTy(ctx_)}, false);
		}
		return functor_type;
	}
	
	IRBuilder<> &operator()() {
		return builder_;
	}


private:
	static inline std::uint64_t get_uid(uint32_t cr3, uint32_t vaddr) {
		return ((uint64_t) cr3 << 32) | vaddr;
	}
	
	static inline std::string get_name(uint64_t uid) {
		return "functor_" + std::to_string(uid);
	}
	
	static inline std::string get_name(uint32_t cr3, uint32_t vaddr) {
		return get_name(get_uid(cr3, vaddr));
	}


private:
	std::unique_ptr<orc::KaleidoscopeJIT> jit_;
	LLVMContext ctx_;
	std::unordered_map<uint64_t, std::pair<RawFT, int>> icache;
	
	static std::optional<int> is_cpu(const rtlreg_t *reg) {
		if ((const rtlreg_t *) &cpu <= reg && reg < (const rtlreg_t *) (&cpu + 1)) {
			return reg - (const rtlreg_t *) &cpu;
		} else {
			return std::nullopt;
		}
	}
	
	Value *get_cpu_reg_ptr(int id) {
		auto reg_ptr = builder_.CreateConstGEP1_32(s_.regfile, id);
		return reg_ptr;
	}
	
	struct State {
		State() {
			this->clear();
		}
		
		int inst_count;
		uint64_t uid;
		std::unique_ptr<Module> mod;
		std::unordered_map<const rtlreg_t *, Value *> value_cache;
		std::array<std::pair<Value *, bool>, sizeof(CPU_state) / sizeof(rtlreg_t)> reg_cache;
		BasicBlock *bb;
		Function *func;
		Value *regfile;
		Value *memory;
		
		void clear() {
			inst_count = 0;
			uid = (uint64_t) -1;
			assert(!mod); // should always been cleared
			value_cache.clear();
			std::fill(reg_cache.begin(), reg_cache.end(), std::make_pair(nullptr, false));
			bb = nullptr;
			func = nullptr;
			regfile = memory = nullptr;
		}
	} s_;
	
	IRBuilder<> builder_;
};
	
}
