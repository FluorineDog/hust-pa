#include "proc.h"
#include "loader.h"
#include "memory.h"

#define MAX_NR_PROC 4

static PCB all_pcbs[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void switch_boot_pcb() {
    current = &pcb_boot;
}

void hello_fun(void *arg) {
    int j = 1;
    while(1) {
        Log("Hello World from Nanos-lite for the %dth time!", j);
        j++;
        _yield();
    }
}

void init_proc() {
    // naive_uload(NULL, "/bin/init");
    context_kload(&all_pcbs[0], (void *)hello_fun);
    // context_kload(&all_pcbs[1], (void *)hello_fun);
    // context_uload(&all_pcbs[0], "/bin/hello");
    context_uload(&all_pcbs[1], "/bin/hello");
    // Log("all_pcbs content: %p", all_pcbs[1].tf->prot->ptr);
    switch_boot_pcb();
}

_Context *schedule(_Context *prev) {
    // record current all_pcbs trapframe
    current->tf = prev;
    // switch to all_pcbs[0]
    static uint32_t n = 0;
    n = (n + 1) & 0xF;
    // Log("Scheduling to %d", n);
    current = (n == 0) ? &all_pcbs[0] : &all_pcbs[1];
    _switch(current->tf);
    return current->tf;
}


int proc_execve(const char *path, char *const argv[], char *const envp[]) {
    // TODO
    // context_uload(&all_pcbs[pcb_iter++], path);
    // panic("wtf");
    TODO();
    return -1;
}

int proc_brk(size_t new_program_break) {
    PCB* pcb = current;
    while(pcb->max_brk < new_program_break){
        assert(PGROUNDDOWN(pcb->max_brk) == pcb->max_brk);
        int ret = _map(&pcb->as, (void*)pcb->max_brk, new_page(1), _PROT_WRITE);
        if(ret != 0) {
            return -1;
        }
        pcb->max_brk += PGSIZE;
    }
    pcb->cur_brk = new_program_break;
    return 0;
}