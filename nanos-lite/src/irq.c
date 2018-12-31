#include "common.h"
#include "proc.h"

_Context* do_syscall(_Context* c);

static _Context* do_event(_Event e, _Context* c) {
    switch(e.event) {
        case _EVENT_YIELD: {
            // printf("[I yield!!]");
            c = schedule(c);
            break;
        }
        case _EVENT_SYSCALL: {
            // Log("irq=%p, pa1=%p, pa2=%p, pa3=%p", c->GPR1, c->GPR2, c->GPR3, c->GPR4);
            c = do_syscall(c);
            break;
        }
        default: panic("Unhandled event ID = %d", e.event);
    }

    return c;
}

void init_irq(void) {
    Log("Initializing interrupt/exception handler...");
    _cte_init(do_event);
}
