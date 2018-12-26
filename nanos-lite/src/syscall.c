#include "syscall.h"
#include "common.h"
#define stdin 0
#define stdout 1
#define stderr 2
extern char _end;

#define _def(variable, NO, type) type variable = (type)c->GPR##NO
#define _def_unused(variable, NO, type) \
    type variable __attribute((__unused__)) = (type)c->GPR##NO

// #define _def(variable, NO) _def_conv(int, variable, NO)
#define _ret() c->GPR1
#define _set_ret(value) _ret() = (int)(size_t)value

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
            // int increment = c->GPR2;
            _def(new_addr, 2, void*);
            if((void*)&_end < new_addr && new_addr < _heap.end) {
                _heap.start = new_addr;
                _set_ret(new_addr);
                break;
                // c->GPR1 = (size_t)old;
            } else {
                void* old_addr = (void*)_heap.start;
                _set_ret(old_addr);
                break;
            }
            break;
        }
        default: panic("Unhandled syscall ID = %d", syscall_type);
    }

    return c;
}
