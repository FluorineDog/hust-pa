#include <am.h>
#include <klib.h>
#include <x86.h>
#include <eflags.h>

static _Context *(*user_handler)(_Event, _Context *) = NULL;

void vectrap();
void vecsys();
void vecnull();
void irq0();

extern void get_cur_as(_Context *c);

_Context *irq_handle(_Context *tf) {
    _Context *next = tf;
    get_cur_as(tf);
    if(user_handler) {
        _Event ev = {0};
        // printf("[fuck with %d]\n", tf->irq);
        switch(tf->irq) {
            case 0x81: {
                // trap
                // printf("[trap]");
                ev.event = _EVENT_YIELD;
                break;
            }
            case 0x80: {
                // syscall
                // printf("[syscall]");
                ev.event = _EVENT_SYSCALL;
                break;
            }
            case 32: {
                ev.event = _EVENT_IRQ_TIMER;
                break;
            }
            case -1: {
                printf("[exit whore]");
                break;
            }
            default: {
                ev.event = _EVENT_ERROR;
                while(1) {
                    printf("damn");
                }
                break;
            }
        }
        // printf("[irq before %p]", tf);
        next = user_handler(ev, tf);
        // printf("[irq after %p]", next);

        if(next == NULL) {
            next = tf;
        }
    }
    _switch(next);
    return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context *(*handler)(_Event, _Context *)) {
    // initialize IDT
    for(unsigned int i = 0; i < NR_IRQ; i++) {
        idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
    }

    // -------------------- system call --------------------------
    idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
    idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_KERN);
    idt[32] = GATE(STS_TG32, KSEL(SEG_KCODE), irq0, DPL_KERN);

    set_idt(idt, sizeof(idt));

    // register event handler
    user_handler = handler;

    return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
    void **args = (void **)stack.end;
    args[0] = arg;
    _Context *ctx = (_Context *)args - 1;
    memset(ctx, 0, sizeof(_Context));
    ctx->eip = (size_t)entry;
    ctx->eflags = MASK_IF;
    ctx->cs = 0x8;
    // TODO WITH ARG
    uintptr_t *tf = (uintptr_t *)stack.start;
    *tf = (uintptr_t)ctx;
    return ctx;
}

void _yield() {
    asm volatile("int $0x81");
}

int _intr_read() {
    return 0;
}

void _intr_write(int enable) {
    if(enable) {
        asm volatile("sti");
    } else {
        asm volatile("cli");
    }
}
