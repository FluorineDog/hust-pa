#include "proc.h"
#include "loader.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
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
    context_kload(&pcb[0], (void* )hello_fun);
    switch_boot_pcb();
}

_Context *schedule(_Context *prev) {
    // WHY? 
    current->tf = prev;
    // switch to pcb[0]
    current = &pcb[0];
    // return tf
    return current->tf;
}

int proc_execve(const char *path, char *const argv[], char *const envp[]) {
    // TODO
    program_naive_uload(NULL, path, argv);
    panic("wtf");
    return -1;
}