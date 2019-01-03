#include "syscall.h"
#include "loader.h"
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"
#include "proc.h"
#include "fs.h"
#define stdin 0
#define stdout 1
#define stderr 2

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
            // _halt(status);
            if(status != 0) {
                _halt(status);
                panic("wtf");
            }
            TODO();
            naive_uload(NULL, "/bin/init");
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
            _def(size, 4, size_t);
            _ret() = vfs_read(fd, buf, size);
            break;
        }
        case SYS_write: {
            // int fd = c->GPR2;
            // const char* buf = (const char*)c->GPR3;
            // int size = c->GPR4;
            _def(fd, 2, int);
            _def(buf, 3, const char*);
            _def(size, 4, size_t);
            _ret() = vfs_write(fd, buf, size);
            break;
        }
        case SYS_close: {
            // TODO();
            _def(fd, 2, int);
            _ret() = vfs_close(fd);
            break;
        }
        case SYS_lseek: {
            _def(fd, 2, int);
            _def(offset, 3, ssize_t);
            _def(whence, 4, int);
            _ret() = vfs_lseek(fd, offset, whence);
            break;
        }
        case SYS_brk: {
            // TODO
            _def(new_prg_brk, 2, size_t);
            _ret() = proc_brk(new_prg_brk);
            break;
        }

        case SYS_execve: {
            _def(path, 2, const char*);
            _def(argv, 3, char* const*);
            _def(envp, 4, char* const*);
            // TODO: i know there is some bug, related to stack, and i won't fix
            c = proc_execve(path, argv, envp);
            _switch(c);
            break;
        }
        default: panic("Unhandled syscall ID = %d", syscall_type);
    }
    // final:
    return c;
}