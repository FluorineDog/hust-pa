#ifndef __ARCH_H__
#define __ARCH_H__

#include <am.h>

#define PMEM_SIZE (128 * 1024 * 1024)
#define PGSIZE 4096    // Bytes mapped by a page

typedef uint32_t size_t;

struct _Context {
    struct _Protect *prot;
    struct {
        uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    };
    int irq;
    uint32_t err;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
