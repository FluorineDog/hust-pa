#include "syscall.h"
#include "common.h"
#define stdin 1

_Context* do_syscall(_Context* c) {
    //   uintptr_t a[4];
    //   a[0] = c->GPR1;
    uint32_t syscall_type = c->GPR1;
    
    switch(syscall_type) {
        case SYS_exit: {
            int status = c->GPR2;
            printf("[status = %d]", status);
            _halt(status);
            break;
        }
        case SYS_yield: {
            _yield();
            c->GPR1 = 0;
            break;
        }
        case SYS_write: {
            int fd = c->GPR2; 
            char* buf = (char *)c->GPR3;
            int size = c->GPR4;
            
            break;
        }
        default: panic("Unhandled syscall ID = %d", syscall_type);
    }

    return c;
}
