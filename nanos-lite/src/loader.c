#include "loader.h"
#include <klib.h>
#include "fs.h"
static uintptr_t loader(PCB *pcb, const char *filename) {
    // fuck the filenames
    // ramdisk_read((void *)DEFAULT_ENTRY, 0, RAMDISK_SIZE);
    assert(filename != NULL);
    int fd = vfs_open(filename, 0, 0);
    int size = vfs_filesz(fd);
    Log("load program %s {fd=%d} with size=%d", filename, fd, size);
    vfs_read(fd, (void *)DEFAULT_ENTRY, size);
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

    pcb->tf = _kcontext(stack, entry, NULL);
}

void extend_page_until(PCB* pcb, int size){
     
}

void context_uload(PCB *pcb, const char *filename) {
    size_t size = vfs_filesz(filename);
    // alloc enough page for pcb

    uintptr_t entry = loader(pcb, filename);

    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);
    
    pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
