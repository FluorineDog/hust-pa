#include <assert.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "syscall.h"

#if defined(__ISA_X86__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2) {
    int ret = -1;
    asm volatile("int $0x80" : "=a"(ret) : "a"(type), "b"(a0), "c"(a1), "d"(a2));
    return ret;
}
#elif defined(__ISA_AM_NATIVE__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2) {
    intptr_t ret = 0;
    asm volatile("call *0x100000" : "=a"(ret) : "a"(type), "S"(a0), "d"(a1), "c"(a2));
    return ret;
}
#else
#error _syscall_ is not implemented
#endif

extern char end;

void _exit(int status) {
    _syscall_(SYS_exit, status, 0, 0);
    while(1)
        ;
}

int _open(const char *path, int flags, mode_t mode) {
    int ret = _syscall_(SYS_open, path, flags, mode);
    return ret;
}

int _write(int fd, void *buf, size_t count) {
    int ret = _syscall_(SYS_write, fd, buf, count);
    return ret;
}

static void *_program_break = &end;
void *_sbrk(intptr_t increment) {
    void *new_pb = _program_break + increment;
    int ret = _syscall_(SYS_brk, new_pb, 0, 0);
    if(ret < 0) {
        assert(0);
    } else {
        void *old_pb = _program_break;
        _program_break = new_pb;
        return old_pb;
    }
}

int _read(int fd, void *buf, size_t count) {
    int ret = _syscall_(SYS_read, fd, buf, count);
    return ret;
}

int _close(int fd) {
    int ret = _syscall_(SYS_close, fd, 0, 0);
    return 0;
}

off_t _lseek(int fd, off_t offset, int whence) {
    int ret = _syscall_(SYS_lseek, fd, offset, whence);
    return ret;
}

int _execve(const char *fname, char *const argv[], char *const envp[]) {
    int ret = _syscall_(SYS_execve, fname, argv, envp);
    return ret;
}

// The code below is not used by Nanos-lite.
// But to pass linking, they are defined as dummy functions

int _fstat(int fd, struct stat *buf) {
    return 0;
}

int _kill(int pid, int sig) {
    _exit(-SYS_kill);
    return -1;
}

pid_t _getpid() {
    _exit(-SYS_getpid);
    return 1;
}

int _fork() {
    extern int errno;
    errno = -1;
    return -1;
}

pid_t _wait(int *status) {
    status = 0;
    return 0;
}
