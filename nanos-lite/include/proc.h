#ifndef __PROC_H__
#define __PROC_H__

#include "common.h"
#include "memory.h"

#define STACK_SIZE (8 * PGSIZE)

typedef union {
    uint8_t stack[STACK_SIZE] PG_ALIGN;
    struct {
        _Context *tf;
        _Protect as;
        uintptr_t cur_brk;
        // we do not free memory, so use `max_brk' to determine when to call _map()
        uintptr_t max_brk;
    };
} PCB;

extern PCB *current;
int proc_execve(const char *path, char *const argv[], char *const envp[]);
int proc_brk(size_t new_program_break);
_Context *schedule(_Context *prev);

// typedef struct _Protect {
//     size_t pgsize;
//     _Area area;
//     void *ptr;
// } _Protect;

#endif
