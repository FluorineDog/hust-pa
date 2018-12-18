#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t *dest, uint8_t subcode) {
	bool invert = subcode & 0x1;
	enum {
		CC_O, CC_NO, CC_B, CC_NB,
		CC_E, CC_NE, CC_BE, CC_NBE,
		CC_S, CC_NS, CC_P, CC_NP,
		CC_L, CC_NL, CC_LE, CC_NLE
	};
	rtlreg_t CF, OF, SF, ZF;
	rtl_get_CF(&CF);
	rtl_get_OF(&OF);
	rtl_get_SF(&SF);
	rtl_get_ZF(&ZF);
	
	// TODO: Query EFLAGS to determine whether the condition code is satisfied.
	// dest <- ( cc is satisfied ? 1 : 0)
	switch (subcode & 0xe) {
		case CC_O:
			// OF = 1
			rtl_mv(dest, &OF);
			break;
		case CC_B:
			// CF = 1
			rtl_mv(dest, &CF);
			break;
		case CC_E:
			// ZF = 1
			rtl_mv(dest, &ZF);
			break;
		case CC_BE:
			// CF = 1 or ZF = 1
			rtl_or(dest, &CF, &ZF);
			break;
		case CC_S:
			// SF = 1
			rtl_mv(dest, &SF);
		case CC_L:
			// SF != OF
			rtl_xor(dest, &SF, &OF);
		case CC_LE:{
			// SF != OF, or ZF = 1
			rtlreg_t tmp;
			rtl_xor(&tmp, &SF, &OF);
			rtl_or(dest, &tmp, &ZF);
		}
		default:
			panic("should not reach here");
		case CC_P:
			panic("n86 does not have PF");
	}
	
	if (invert) {
		rtl_xori(dest, dest, 0x1);
	}
}
