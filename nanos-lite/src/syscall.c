#include "syscall.h"
#include "common.h"
#define stdin 0
#define stdout 1
#define stderr 2

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
            char* buf = (char*)c->GPR3;
            int size = c->GPR4;
            // printf("[write_fd = %d]", fd);
            assert(fd > 0);
            assert(fd == 1);
            if(fd <= 3) {
                // special file
                // printf("[size = $%d]", size);
                // assert(fd == 1);

                int ref = printf("%s", buf);
                assert(true || ref == size);
                c->GPR1 = ref;
            } else {
            }
            break;
        }
        case SYS_brk: {
            int increment = c->GPR2;
            void* old = (void*) _heap.start;
            void* new = old + increment;
            if(new >= _heap.end) {
                c->GPR1 = -1;
                break;
            } else {
                _heap.start = new;
                c->GPR1 = (uint32_t) old;
                break;
            }
        }
        default: panic("Unhandled syscall ID = %d", syscall_type);
    }

    return c;
}
