#include "jit_rtl.h"

void jit_rtl_li(rtlreg_t *dest, uint32_t imm) {
	JIT_DONE;
	*dest = imm;
	
	JIT_COMPILE_BARRIER;
	auto vimm = eng().getInt32(imm);
	eng.set_value(dest, vimm);
}

void jit_rtl_mv(rtlreg_t *dest, const rtlreg_t *src1) {
	JIT_DONE;
	*dest = *src1;
	
	JIT_COMPILE_BARRIER;
	auto val = eng.get_value(src1);
	eng.set_value(dest, val);
}

//make_rtl_arith_logic_dog(add)
void jit_rtl_add(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 + *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateAdd(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(sub)
void jit_rtl_sub(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 - *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateSub(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(and)
void jit_rtl_and(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 & *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateAnd(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(or)
void jit_rtl_or(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 | *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateOr(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(xor)
void jit_rtl_xor(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 ^ *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateXor(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(shl)
void jit_rtl_shl(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 << *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateShl(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(shr)
void jit_rtl_shr(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = *src1 >> *src2;
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateLShr(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(sar)
void jit_rtl_sar(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	*dest = (uint32_t) ((int32_t) *src1 >> *src2);
	
	JIT_COMPILE_BARRIER;
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto vres = eng().CreateAShr(va, vb);
	eng.set_value(dest, vres);
}

//make_rtl_arith_logic_dog(mul_lo);

inline std::pair<llvm::Value*, llvm::Value*> get_integer_pair(const rtlreg_t *src1, const rtlreg_t *src2, bool is_sign){
	auto va = eng.get_value(src1);
	auto vb = eng.get_value(src2);
	auto va64 = eng().CreateIntCast(va, eng().getInt64Ty(), is_sign);
	auto vb64 = eng().CreateIntCast(vb, eng().getInt64Ty(), is_sign);
	return std::make_pair(va64, vb64);
}

inline void set_value64(rtlreg_t *dest, llvm::Value* v64){
	auto v32 = eng().CreateIntCast(v64, eng().getInt32Ty(), false);
	eng.set_value(dest, v32);
}

void jit_rtl_mul_lo(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	uint64_t x = *src1;
	uint64_t y = *src2;
	uint64_t res = (x * y) >> 0;
	*dest = (uint32_t)res;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, false);
	auto vres64 = eng().CreateMul(va64, vb64);
	set_value64(dest, vres64);
}

//make_rtl_arith_logic_dog(mul_hi);
void jit_rtl_mul_hi(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	uint64_t x = *src1;
	uint64_t y = *src2;
	uint64_t res = (x * y) >> 32;
	*dest = (uint32_t)res;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, false);
	auto vmul64 = eng().CreateMul(va64, vb64);
	auto vres64 = eng().CreateLShr(vmul64, 32);
	set_value64(dest, vres64);
}

//make_rtl_arith_logic_dog(imul_lo);
void jit_rtl_imul_lo(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	int64_t x = (int32_t)*src1;
	int64_t y = (int32_t)*src2;
	int64_t res = (x * y) >> 0;
	*dest = (uint32_t)res;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, true);
	auto vres64 = eng().CreateMul(va64, vb64);
	set_value64(dest, vres64);
}

//make_rtl_arith_logic_dog(imul_hi);
void jit_rtl_imul_hi(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	int64_t x = (int32_t)*src1;
	int64_t y = (int32_t)*src2;
	int64_t res = (x * y) >> 32;
	*dest = (uint32_t)res;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, true);
	auto vmul64 = eng().CreateMul(va64, vb64);
	auto vres64 = eng().CreateAShr(vmul64, 32);
	set_value64(dest, vres64);
}

//make_rtl_arith_logic_dog(div_q);
void jit_rtl_div_q(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	uint64_t x = *src1;
	uint64_t y = *src2;
	uint64_t quot = x / y;
//	uint64_t rem  = x % y;
	*dest = (uint32_t)quot;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, false);
	auto vdiv64 = eng().CreateUDiv(va64, vb64);
	set_value64(dest, vdiv64);
}

//make_rtl_arith_logic_dog(div_r);
void jit_rtl_div_r(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	uint64_t x = *src1;
	uint64_t y = *src2;
//	uint64_t quot = x / y;
	uint64_t rem  = x % y;
	*dest = (uint32_t)rem;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, false);
	auto vrem64 = eng().CreateURem(va64, vb64);
	set_value64(dest, vrem64);
}

//make_rtl_arith_logic_dog(idiv_q);
void jit_rtl_idiv_q(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	int64_t x = (int32_t)*src1;
	int64_t y = (int32_t)*src2;
	int64_t quot = x / y;
//	int64_t rem  = x % y;
	*dest = (uint32_t)quot;
	
	JIT_COMPILE_BARRIER;
	
	auto [va64, vb64] = get_integer_pair(src1, src2, true);
	auto vdiv64 = eng().CreateSDiv(va64, vb64);
	set_value64(dest, vdiv64);
}

//make_rtl_arith_logic_dog(idiv_r);
void jit_rtl_idiv_r(rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) {
	JIT_DONE;
	int64_t x = (int32_t)*src1;
	int64_t y = (int32_t)*src2;
//	int64_t quot = x / y;
	int64_t rem  = x % y;
	*dest = (uint32_t)rem;
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_integer_pair(src1, src2, true);
	auto vrem64 = eng().CreateSRem(va64, vb64);
	set_value64(dest, vrem64);
}

inline std::pair<llvm::Value*, llvm::Value*> get_i64_pair(
		const rtlreg_t *src1_lo, const rtlreg_t *src1_hi, const rtlreg_t *src2, bool is_sign){
	auto va_lo = eng.get_value(src1_lo);
	auto va_hi = eng.get_value(src1_hi);
	auto vb = eng.get_value(src2);
	auto va64_lo = eng().CreateIntCast(va_lo, eng().getInt64Ty(), false);
	auto va64_hi_raw = eng().CreateIntCast(va_hi, eng().getInt64Ty(), false);
	auto va64_hi = eng().CreateShl(va64_hi_raw, 32);
	auto va64_u = eng().CreateOr(va64_lo, va64_hi);
	
	auto va64 = eng().CreateIntCast(va64_u, eng().getInt64Ty(), is_sign);
	auto vb64 = eng().CreateIntCast(vb, eng().getInt64Ty(), is_sign);
	return std::make_pair(va64, vb64);
}

void jit_rtl_div64_q(rtlreg_t *dest, const rtlreg_t *src1_hi,
		const rtlreg_t *src1_lo, const rtlreg_t *src2) {
	JIT_DONE;
	uint64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = (uint32_t) (dividend / divisor);
	
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_i64_pair(src1_lo, src1_hi, src2, false);
	auto vres64 = eng().CreateUDiv(va64, vb64);
	set_value64(dest, vres64);
}

void jit_rtl_div64_r(rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo,
		const rtlreg_t *src2) {
	JIT_DONE;
	uint64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = (uint32_t) (dividend % divisor);
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_i64_pair(src1_lo, src1_hi, src2, false);
	auto vres64 = eng().CreateURem(va64, vb64);
	set_value64(dest, vres64);
}

void jit_rtl_idiv64_q(rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo,
		const rtlreg_t *src2) {
	JIT_DONE;
	int64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = (uint32_t) (dividend / divisor);
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_i64_pair(src1_lo, src1_hi, src2, true);
	auto vres64 = eng().CreateSDiv(va64, vb64);
	set_value64(dest, vres64);
}

void jit_rtl_idiv64_r(rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo,
		const rtlreg_t *src2) {
	JIT_DONE;
	int64_t dividend = ((uint64_t) (*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = (uint32_t) (dividend % divisor);
	
	JIT_COMPILE_BARRIER;
	auto [va64, vb64] = get_i64_pair(src1_lo, src1_hi, src2, true);
	auto vres64 = eng().CreateSRem(va64, vb64);
	set_value64(dest, vres64);
}