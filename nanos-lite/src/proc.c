#include "proc.h"
#include "loader.h"

#define MAX_NR_PROC 6

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
static int pcb_iter = 2;
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
    context_kload(&pcb[0], (void *)hello_fun);
    context_kload(&pcb[1], (void *)hello_fun);
    // context_uload(&pcb[1], "/bin/init");
    switch_boot_pcb();
}

_Context *schedule(_Context *prev) {
    // record current pcb trapframe
    current->tf = prev;
    // switch to pcb[0]
    static uint32_t n = 0;
    n = (n + 1) & 0xF;
    current = (n == 0) ? &pcb[0] : &pcb[1];
    _switch(current->tf);
    return current->tf;
}

int proc_execve(const char *path, char *const argv[], char *const envp[]) {
    // TODO
    context_uload(&pcb[pcb_iter++], path);
    panic("wtf");
    return -1;
}