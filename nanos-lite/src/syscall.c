#include "syscall.h"
#include "fs.h"
#include "common.h"
#define stdin 0
#define stdout 1
#define stderr 2
extern char _end;

_Context* do_syscall(_Context* c) {
    //   uintptr_t a[4];
    //   a[0] = c->GPR1;
    uint32_t syscall_type = c->GPR1;
    // printf("[do_sys %d]", syscall_type);

    switch(syscall_type) {
        case SYS_exit: {
            int status = c->GPR2;
            // printf("[status = %d]", status);
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
            const char* buf = (const char*)c->GPR3;
            int size = c->GPR4;
            int ret = vfs_write(fd, buf, size); 
            c->GPR1 = ret;
            break;
        }
        case SYS_read: {
            int fd = c->GPR2;
            char* buf = (char*)c->GPR3;
            int size = c->GPR4;
            int ret = vfs_read(fd, buf, size); 
            c->GPR1 = ret;
            break;
        }
        case SYS_brk: {
            int increment = c->GPR2;
            void* old = (void*)_heap.start;
            void* new = old + increment;
            if((void*)&_end < new&& new < _heap.end) {
                _heap.start = new;
                c->GPR1 = (size_t)old;
                break;
            } else {
                c->GPR1 = -1;
                break;
            }
        }
        default: panic("Unhandled syscall ID = %d", syscall_type);
    }

    return c;
}
