#include "syscall.h"
#include "common.h"

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
            
            break;
        }
        default: panic("Unhandled syscall ID = %d", syscall_type);
    }

    return c;
}
