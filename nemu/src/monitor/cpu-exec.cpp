#include "nemu.h"
#include "device/device.h"
#include "monitor/monitor.h"
#include "monitor/watchpoint.h"
#include "monitor/diff-test.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 20

int nemu_state = NEMU_STOP;

void exec_wrapper(bool);

uint64_t g_nr_guest_instr = 0;

void nr_guest_instr_add(uint32_t n) {
    g_nr_guest_instr += n;
}

void monitor_statistic() {
    Log("total guest instructions = %ld", g_nr_guest_instr);
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
    if (nemu_state == NEMU_END || nemu_state == NEMU_ABORT) {
        printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
        return;
    }
    nemu_state = NEMU_RUNNING;

    bool print_flag = n <= MAX_INSTR_TO_PRINT;

    for (; n > 0; n--) {
        /* Execute one instruction, including instruction fetch,
         * instruction decode, and the actual execution. */
        exec_wrapper(print_flag);
        nr_guest_instr_add(1);


#ifdef HAS_IOE
        device_update();
#endif

        if (nemu_state != NEMU_RUNNING) {
            if (nemu_state == NEMU_END) {
                printflog("\33[1;31mnemu: HIT %s TRAP\33[0m at eip = 0x%08x\n\n",
                          (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip - 1);
                monitor_statistic();
                return;
            } else if (nemu_state == NEMU_ABORT) {
                printflog("total count of guest instructions = %ld\n", g_nr_guest_instr);
                printflog("\33[1;31mnemu: ABORT\33[0m at eip = 0x%08x\n\n", cpu.eip);
                return;
            }
        }
#ifdef DEBUG
        {

            bool changed = false;
            for (auto&[id, wp]: g_watch_point_pool) {
                int old = wp.get_value();
                if (wp.update()) {
                    int new_v = wp.get_value();
                    changed = true;
                    printf("Software watchpoint %d: %s\n",
                           id, wp.get_expr_str().c_str());
                    printf("Old value = %d [0x%08x]\n", old, old);
                    printf("New value = %d [0x%08x]\n", new_v, new_v);
                    printf("\n");
                }
            }
            if (changed && nemu_state == NEMU_RUNNING) {
                nemu_state = NEMU_STOP;
                return;
            }
        }
#endif
    }

    if (nemu_state == NEMU_RUNNING) { nemu_state = NEMU_STOP; }
}

#include <fstream>
#include "device/mmio.h"
using std::ofstream;
using std::ifstream;
constexpr uint64_t magic_number = 0x12d8b5d9FFBBCCDD;
void image_save(const char* filename){
    Log("image saving");
    ofstream fout(filename, std::ios::binary| std::ios::out);
    fout.write((const char*)&magic_number, sizeof(magic_number));
    fout.write((const char*)&cpu, sizeof(cpu));
    fout.write((const char*)pmem, PMEM_SIZE);
    save_mmio(fout);
}

void image_load(const char* filename) {
	Log("image loading");
	sdl_clear_event_queue();
    ifstream fin(filename, std::ios::binary| std::ios::in);
    uint64_t magic;
    fin.read((char*)&magic, sizeof(magic));
    if(magic != magic_number){
        panic("fuck you, this is not an image file");
    }
    fin.read((char*)&cpu, sizeof(cpu));
    fin.read((char*)pmem, PMEM_SIZE);
	load_mmio(fin);
    device_update(true);
    if(g_diff_test_enabled){
        difftest_recover();
    }
}


