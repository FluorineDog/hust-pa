#include "trap.h"

char buf[128];

int main() {
	sprintf(buf, "%s", "Hello world!\n");
	nemu_assert(strcmp(buf, "Hello world!\n") == 0);

	sprintf(buf, "%x + %08x =%3x\n", 0xabcdef00, 0xfe, 0x12);
	nemu_assert(strcmp(buf, "abcdef00 + 000000fe = 12\n") == 0);

	sprintf(buf, "%d + %d = %d\n", 2, 10, 12);
	nemu_assert(strcmp(buf, "2 + 10 = 12\n") == 0);

	return 0;
}
