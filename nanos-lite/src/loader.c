#include "loader.h"
#include "fs.h"
#include <klib.h>
static uintptr_t loader(PCB *pcb, const char *filename) {
    // fuck the filenames
    // ramdisk_read((void *)DEFAULT_ENTRY, 0, RAMDISK_SIZE);
    assert(filename != NULL);
    int fd = vfs_open(filename, 0, 0);
    int size = vfs_filesz(fd); 
    Log("load program %s {fd=%d} with size=%d", filename, fd, size);
    vfs_read(fd, (void*)DEFAULT_ENTRY, size);
    vfs_close(fd);
    return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
    uintptr_t entry = loader(pcb, filename);
    ((void (*)())entry)();
    panic("wtf");
}

void context_kload(PCB *pcb, void *entry) {
    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);

    pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
    uintptr_t entry = loader(pcb, filename);

    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);

    pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
