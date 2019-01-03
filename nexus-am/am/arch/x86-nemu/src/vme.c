#include <eflags.h>
#include <x86.h>
#include <klib.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void *(*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void *);

_Area segments[] = {    // Kernel memory mappings
    {.start = (void *)0, .end = (void *)PMEM_SIZE}};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void *(*pgalloc_f)(size_t), void (*pgfree_f)(void *)) {
    pgalloc_usr = pgalloc_f;
    pgfree_usr = pgfree_f;

    // make all PDEs invalid
    for(int i = 0; i < NR_PDE; i++) {
        kpdirs[i] = 0;
    }

    PTE *ptab = kptabs;
    for(int i = 0; i < NR_KSEG_MAP; i++) {
        uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
        uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
        for(; pdir_idx < pdir_idx_end; pdir_idx++) {
            // fill PDE
            kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

            // fill PTE
            PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
            PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
            for(; pte < pte_end; pte += PGSIZE) {
                *ptab = pte;
                ptab++;
            }
        }
    }

    set_cr3(kpdirs);
    set_cr0(get_cr0() | CR0_PG);

    return 0;
}

int _protect(_Protect *p) {
    PDE *updir = (PDE *)(pgalloc_usr(1));
    p->pgsize = 4096;
    p->ptr = updir;
    // map kernel space
    for(int i = 0; i < NR_PDE; i++) {
        // low level page: use kernel
        updir[i] = kpdirs[i];
    }
    p->area.start = (void *)0x8000000;
    p->area.end = (void *)0xc0000000;
    return 0;
}

void _unprotect(_Protect *p) {
    // DO NOTHING?
    // discard the fuck
    // to avoid anything
    memset(p, sizeof(_Protect), 0);
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c) {
    c->prot = cur_as;
}

void _switch(_Context *c) {
    assert(c->prot);
    set_cr3(c->prot->ptr);
    cur_as = c->prot;
}

int _map(_Protect *p, void *va, void *pa, int mode) {
    // TODO
    uint32_t pdx = PDX(va);

    PDE *page_dir = (PDE *)p->ptr;
    assert(page_dir);
    if((page_dir[pdx] & PTE_P) == 0) {
        assert(page_dir[pdx] == 0);
        // page table is missing
        // create new page
        PTE *pt = (PTE *)pgalloc_usr(1);
        assert(((uint32_t)pt & 0x3ff) == 0);
        PDE new_pde = PTE_P | (uint32_t)pt;
        page_dir[pdx] = new_pde;
    }
    PDE pde = page_dir[pdx];
    PTE *page_table = (PTE *)PDE_ADDR(pde);

    uint32_t ptx = PTX(va);
    if((page_table[ptx] & PTE_P) == 0) {
        assert(page_table[ptx] == 0);
        page_table[ptx] = PTE_P | (uint32_t)pa;
    }
    return 0;
}

// construct a valid "context"/ trapframe on ustack from given parameters
_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry,
                    void *args_raw_void) {
    // TODO
    int argc = 0;
    char **stack_argv = (char **)ustack.end;
    const char* const* args_raw = (const char* const*)args_raw_void;
    if(args_raw) {
        while(args_raw[argc]) {
            argc++;
            stack_argv--;
        }
        ustack.end = stack_argv;
    }

    if(args_raw){
        char *stack_strs = (char *)ustack.end;
        for(int i = 0; i < argc; ++i) {
            int len = strlen(args_raw[i]) + 1;
            stack_strs -= len;
            strcpy(stack_strs, args_raw[i]);
            stack_argv[i] = stack_strs;
        }
        ustack.end = stack_strs;
    }

    {
        uint32_t *stack_args = (uint32_t *)ustack.end - 4;
        // placeholder for return addr of _start
        stack_args[0] = 0; 
        // parameters of _start
        stack_args[1] = argc;
        stack_args[2] = (uint32_t)stack_argv;
        stack_args[3] = 0;
        ustack.end = stack_args;
        // printf("[%p fuck %s]", stack_argv,  stack_argv[1]);
    }

    _Context *ctx = (_Context *)ustack.end - 1;
    {
        memset(ctx, sizeof(_Context), 0);
        ctx->prot = p;
        ctx->eip = (uint32_t)entry;
        ctx->cs = 0x8;
        ctx->eflags = MASK_IF;
        ustack.end = ctx;
    }

    {
        // set stacktop with tf
        *(uintptr_t *)ustack.start = (uintptr_t)ustack.end;
    }
    return ctx;
}
