#include <dlfcn.h>

#include "cpu/eflags.h"
#include "monitor/diff-test.h"
#include "monitor/monitor.h"
#include "nemu.h"

uint32_t g_ignore_eflags = 0;
bool g_diff_test_enabled = true;

static void (*ref_difftest_memcpy_from_dut)(paddr_t dest, void *src, size_t n);

static void (*ref_difftest_getregs)(void *c);

static void (*ref_difftest_setregs)(const void *c);

static void (*ref_difftest_exec)(uint64_t n);

static void (*ref_difftest_loadidt)(uint16_t limit, uint32_t base);

static bool is_skip_ref;
static bool is_skip_dut;

void difftest_skip_ref() {
    is_skip_ref = true;
}

void difftest_skip_dut() {
    is_skip_dut = true;
}

void init_difftest(const char *ref_so_file, long img_size) {
#ifndef DIFF_TEST
    return;
#endif

    assert(ref_so_file != NULL);

    void *handle;
    handle = dlopen(ref_so_file, RTLD_LAZY | RTLD_DEEPBIND);
    assert(handle);

    BITCAST(ref_difftest_memcpy_from_dut, dlsym(handle, "difftest_memcpy_from_dut"));
    assert(ref_difftest_memcpy_from_dut);

    BITCAST(ref_difftest_getregs, dlsym(handle, "difftest_getregs"));
    assert(ref_difftest_getregs);

    BITCAST(ref_difftest_setregs, dlsym(handle, "difftest_setregs"));
    assert(ref_difftest_setregs);

    BITCAST(ref_difftest_exec, dlsym(handle, "difftest_exec"));
    assert(ref_difftest_exec);
    
    BITCAST(ref_difftest_loadidt, dlsym(handle, "difftest_loadidt"));
    assert(ref_difftest_loadidt);

    void (*ref_difftest_init)(void);
    BITCAST(ref_difftest_init, dlsym(handle, "difftest_init"));
    assert(ref_difftest_init);

    Log("Differential testing: \33[1;32m%s\33[0m", "ON");
    Log("The result of every instruction will be compared with %s. "
        "This will help you a lot for debugging, but also significantly reduce the "
        "performance. "
        "If it is not necessary, you can turn it off in include/common.h.",
        ref_so_file);

    ref_difftest_init();
    ref_difftest_memcpy_from_dut(ENTRY_START, guest_to_host(ENTRY_START), img_size);
    ref_difftest_setregs(&cpu);
}

void difftest_step(uint32_t eip, int n) {
    if(!g_diff_test_enabled) {
        return;
    }
    CPU_state ref_cpu;

    if(is_skip_dut) {
        is_skip_dut = false;
        return;
    }

    if(is_skip_ref) {
        // to skip the checking of an instruction, just copy the reg state to reference design
        ref_difftest_setregs(&cpu);
        is_skip_ref = false;
        return;
    }

    ref_difftest_exec(n);
    ref_difftest_getregs(&ref_cpu);

    // TODO: Check the registers state with the reference design.
    // Set `nemu_state` to `NEMU_ABORT` if they are not the same.
    bool failed = false;
    for(int i = 0; i < 8; ++i) {
        auto ref_value = ref_cpu.gpr[i]._32;
        auto value = cpu.gpr[i]._32;
        if(ref_value != value) {
            failed = true;
        }
    }
    failed = failed || cpu.eip != ref_cpu.eip;
    using namespace EFLAGS;
    constexpr auto main_checker = MASK_OF | MASK_CF | MASK_SF | MASK_ZF;
    uint32_t test_flags = main_checker;
    test_flags &= ~g_ignore_eflags;
    failed = failed || ((cpu.eflags ^ ref_cpu.eflags) & test_flags) != 0;
    // if not the same, continue to ignore
    g_ignore_eflags &= (cpu.eflags ^ ref_cpu.eflags);

    auto debug_print = [&](const char *name, uint32_t value, uint32_t ref) {
        printf("%-8s0x%08x%16d [0x%08x%16d] %s\n", name, value, value, ref, ref,
               value != ref ? "*" : "");
    };

    if(failed) {
        for(int i = 0; i < 8; ++i) {
            auto value = cpu.gpr[i]._32;
            auto ref = ref_cpu.gpr[i]._32;
            debug_print(regsl[i], value, ref);
        }
        //		printf("%-8s0x%08x%16d\n", "eip", cpu.eip, cpu.eip);
        debug_print("eip", cpu.eip, ref_cpu.eip);
        printf("ZF=%d[%d], SF=%d[%d], CF=%d[%d], OF=%d[%d]\n", get_ZF(cpu.eflags),
               get_ZF(ref_cpu.eflags), get_SF(cpu.eflags), get_SF(ref_cpu.eflags),
               get_CF(cpu.eflags), get_CF(ref_cpu.eflags), get_OF(cpu.eflags),
               get_OF(ref_cpu.eflags));
        nemu_state = NEMU_ABORT;
    }

}


void difftest_recover(){
    ref_difftest_loadidt((uint16_t)cpu.idtr.limit, cpu.idtr.base);
    ref_difftest_memcpy_from_dut(0, guest_to_host(0), 0x7c00);
    ref_difftest_memcpy_from_dut(ENTRY_START, guest_to_host(ENTRY_START), PMEM_SIZE - ENTRY_START);
    ref_difftest_setregs(&cpu);
}


