#ifndef __ARCH_H__
#define __ARCH_H__

#include <am.h>

#define PMEM_SIZE (128 * 1024 * 1024)
#define PGSIZE 4096    // Bytes mapped by a page

typedef uint32_t size_t;
typedef int32_t ssize_t;

struct _Context {
    // always wrong !!!
    struct _Protect *prot;
    struct {
        size_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    };
    ssize_t irq;
    size_t err;
    size_t eip;
    size_t cs;
    size_t eflags;
};

#define GPR1 eax
#define GPR2 ebx
#define GPR3 ecx
#define GPR4 edx
#define GPRx eip

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
