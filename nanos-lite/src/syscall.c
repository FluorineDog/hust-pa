#include "syscall.h"
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"
#include "fs.h"
#define stdin 0
#define stdout 1
#define stderr 2
extern char _end;

#define _def(variable, NO, type) type variable = (type)c->GPR##NO
#define _def_unused(variable, NO, type) \
    type variable __attribute((__unused__)) = (type)c->GPR##NO

// #define _def(variable, NO) _def_conv(int, variable, NO)
#define _ret() c->GPR1
#define _set_ret(value) _ret() = (ssize_t)value

_Context* do_syscall(_Context* c) {
    // uint32_t syscall_type = c->GPR1;
    _def(syscall_type, 1, ssize_t);

    switch(syscall_type) {
        case SYS_exit: {
            // int status = c->GPR2;
            _def(status, 2, int);
            // printf("[status = %d]", status);
            _halt(status);
            break;
        }
        case SYS_yield: {
            _yield();
            _set_ret(0);
            break;
        }
        case SYS_open: {
            _def(filename, 2, const char*);
            _def(flags, 3, int);
            _def(mode, 4, mode_t);
            _ret() = vfs_open(filename, flags, mode);
            break;
        }
        case SYS_read: {
            // int fd = c->GPR2;
            // char* buf = (char*)c->GPR3;
            // int size = c->GPR4;
            _def(fd, 2, int);
            _def(buf, 3, char*);
            _def(size, 4, int);
            _ret() = vfs_read(fd, buf, size);
            break;
        }
        case SYS_write: {
            // int fd = c->GPR2;
            // const char* buf = (const char*)c->GPR3;
            // int size = c->GPR4;
            _def(fd, 2, int);
            _def(buf, 3, const char*);
            _def(size, 4, int);
            _ret() = vfs_write(fd, buf, size);
            break;
        }
        // TODO
        case SYS_close: {
            // TODO();
            _def(fd, 2, int);
            _ret() = vfs_close(fd);
            break;
        }
        case SYS_lseek: {
            _def(fd, 2, int);
            _def(offset, 3, off_t);
            _def(whence, 4, int);
            _ret() = vfs_lseek(fd, offset, whence);
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
    // final:
    return c;
}
