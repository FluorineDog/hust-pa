#include "loader.h"
#include <klib.h>
static uintptr_t loader(PCB *pcb, const char *filename) {
    // fuck the filenames
    ramdisk_read((void *)DEFAULT_ENTRY, 0, RAMDISK_SIZE);
    // TODO();
    return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
    uintptr_t entry = loader(pcb, filename);
    ((void (*)())entry)();
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
