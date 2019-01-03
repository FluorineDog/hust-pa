#include "loader.h"
#include <am.h>
#include <klib.h>
#include "fs.h"
static inline size_t min(size_t a, size_t b) {
    return a < b ? a : b;

}
static uintptr_t loader(PCB *pcb, const char *filename) {
    assert(filename != NULL);
    // alloc enough page for pcb
    int fd = vfs_open(filename, 0, 0);
    int size = vfs_filesz(fd);
    Log("load program %s {fd=%d} with size=%d", filename, fd, size);
    Log("allocing pages", filename, fd, size);
    
    // alloc pages for
    for(int iter = 0; iter < PGROUNDUP(size); iter += PGSIZE) {
        void *va = (void *)DEFAULT_ENTRY + iter;
        void *pa = new_page(1);
        _map(&pcb->as, va, pa, _PROT_WRITE);
        vfs_read(fd, pa, min(PGSIZE, size - iter));
    }
    pcb->max_brk = DEFAULT_ENTRY + PGROUNDUP(size);
    pcb->cur_brk = DEFAULT_ENTRY + size;

    vfs_close(fd);
    return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
    uintptr_t entry = loader(pcb, filename);
    ((void (*)())entry)();
    panic("wtf");
}

static char buf[3][128];
const char *argv[3] = {buf[0], buf[1], buf[2]};
void program_naive_uload(PCB *pcb, const char *filename, char *const *raw_argv) {
    int i = 0;
    while(raw_argv[i] != NULL && i < 3) {
        strcpy(buf[i], raw_argv[i]);
        i++;
    }
    uintptr_t entry = loader(pcb, filename);
    ((void (*)(int argc, const char *const *argv))entry)(i, argv);
    panic("wtf");
}

void context_kload(PCB *pcb, void *entry) {
    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);
    _protect(&pcb->as);
    pcb->tf = _kcontext(stack, entry, NULL);
    pcb->tf->prot = &pcb->as;
    assert(pcb->tf->prot == &pcb->as);
}

void context_uload(PCB *pcb, const char *filename) {
    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);
    _protect(&pcb->as);
    uintptr_t entry = loader(pcb, filename);
    pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
    assert(pcb->tf->prot == &pcb->as);
    assert(pcb->tf->prot);
    // assert(pcb->tf->prot->ptr);
}
