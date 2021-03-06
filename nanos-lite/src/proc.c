#include "proc.h"
#include "loader.h"
#include "memory.h"


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
    // const char* args[] = {"/bin/litenes", "/share/games/nes/mario.nes", NULL};
    // const char* args2[] = {"/bin/litenes", "/share/games/nes/kungfu.nes", NULL};
    context_kload(&all_pcbs[0], (void *)hello_fun);
    // context_kload(&all_pcbs[1], (void *)hello_fun);
    // context_uload(&all_pcbs[0], "/bin/hello");
    // context_uload(&all_pcbs[1], "/bin/litenes", args);
    context_uload(&all_pcbs[1], "/bin/init", NULL);
    context_uload(&all_pcbs[2], "/bin/init", NULL);
    context_uload(&all_pcbs[3], "/bin/init", NULL);
    // Log("all_pcbs content: %p", all_pcbs[1].tf->prot->ptr);
    switch_boot_pcb();
}
static int chosen_pcb_id = 1;
void set_pcb_id(int id){
    // Log("switching to %d", id);
    // delayed
    chosen_pcb_id = id;
    Log("chosen=%d", id);
}

int get_pcb_id(){
    int id = current - all_pcbs;
    if((uint32_t)id >= (uint32_t)MAX_NR_PROC){
        id = 0;
    }
    // assert(0<= id && id < MAX_NR_PROC);
    return id;
}

_Context *schedule(_Context *prev) {
    // record current all_pcbs trapframe
    current->tf = prev;
    // switch to all_pcbs[0]
    static uint32_t n = 0;
    n = (n + 1) & 0x1F;
    // n = 1;
    // Log("Scheduling to %d", n);
    current = (n == 0) ? &all_pcbs[0] : &all_pcbs[chosen_pcb_id];
    if(chosen_pcb_id ==0 ) chosen_pcb_id = 1;
    _switch(current->tf);
    return current->tf;
}


_Context* proc_execve(const char *path, char *const argv[], char *const envp[]) {
    // TODO
    // context_uload(&all_pcbs[pcb_iter++], path);
    // panic("wtf");
    // TODO();
    PCB* pcb = current;
    // memset(pcb, 0, sizeof(PCB));
    _unprotect(&pcb->as);
    context_uload(pcb, path, (void*)argv);
    return pcb->tf;
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