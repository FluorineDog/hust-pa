#include "cpu/relop.h"
#include "common.h"
bool interpret_relop(uint32_t relop, const rtlreg_t src1_raw, const rtlreg_t src2_raw, int width) {
    int offset = (4 - width) * 8;
    uint32_t src1 = src1_raw << offset;
    uint32_t src2 = src2_raw << offset;
    switch(relop) {
        case RELOP_FALSE: return false;
        case RELOP_TRUE: return true;
        case RELOP_EQ: return src1 == src2;
        case RELOP_NE: return src1 != src2;
        case RELOP_LT: return (int32_t)src1 < (int32_t)src2;
        case RELOP_LE: return (int32_t)src1 <= (int32_t)src2;
        case RELOP_GT: return (int32_t)src1 > (int32_t)src2;
        case RELOP_GE: return (int32_t)src1 >= (int32_t)src2;
        case RELOP_LTU: return src1 < src2;
        case RELOP_LEU: return src1 <= src2;
        case RELOP_GTU: return src1 > src2;
        case RELOP_GEU: return src1 >= src2;
        default: panic("unsupport relop = %d", relop);
    }
}
