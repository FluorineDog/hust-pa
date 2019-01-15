#pragma once
#include "cpu/jit.h"
#include "workflow.h"
#include "cpu/exec.h"

#define JIT_COMPILE_FLAG 1

#if JIT_COMPILE_FLAG
#define JIT_COMPILE_BARRIER
//#define JIT_HEADER do{ fprintf(stderr, "%s\n", __FUNCTION__); }while(0)
//#define JIT_TODO do{fprintf(stderr, "todo: ");JIT_HEADER; assert(0 + 0);}while(0)

#define JIT_HEADER do{  }while(0)
#define JIT_TODO do{ assert(0 + 0);}while(0)
#else
#define JIT_COMPILE_BARRIER return
#define JIT_HEADER do{  }while(0)
#define JIT_TODO do{ assert(0 + 1);}while(0)
#endif

#define JIT_DONE JIT_HEADER
namespace jit {

enum class JITState {
	Init,
	Compiling,
	Terminate,
};

extern JITState state_;
} // jit

extern llvm::CodeExecutor eng;
