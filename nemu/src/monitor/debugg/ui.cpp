#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "wheel/string_tools.h"
#include "nemu.h"
#include "cpu/eflags.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "monitor/diff-test.h"
#include "device/device.h"

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char *rl_gets() {
	static char *line_read = nullptr;
	
	if (line_read) {
		free(line_read);
		line_read = nullptr;
	}
	printf("[%ld]", g_nr_guest_instr);
	line_read = readline("(nemu) ");
	
	if (line_read && *line_read) {
		add_history(line_read);
	}
	
	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_restart(char *args) {
	void restart();
	restart();
	cpu_exec(-1);
	return 0;
}


static int cmd_help(char *args);

static int cmd_step_into(char *args);

static int cmd_info(char *args);

static int cmd_eval(char *args);

static int cmd_scan_memory(char *args);

static int cmd_scan_memory_n(char *args);

static int cmd_scan_physical_memory(char *args);

static int cmd_scan_physical_memory_n(char *args);

static int cmd_add_watch(char *args);

static int cmd_delete_watch(char *args);

static int cmd_save_image(char *args);

static int cmd_load_image(char *args);

static int cmd_attach_difftest(char *args);

static int cmd_detach_difftest(char *args);


static struct {
	const char *name;
	const char *description;
	
	int (*handler)(char *);
} cmd_table[] = {
		{"help",   "Display informations about all supported commands", cmd_help},
		{"c",      "Continue the execution of the program",             cmd_c},
		{"q",      "Exit NEMU",                                         cmd_q},
		{"si",     "Step [N] instruction(s)",                           cmd_step_into},
		{"info",   "Provide information of (r)egister/(w)atchpoint",    cmd_info},
		{"p",      "Eval expression",                                   cmd_eval},
		{"x",      "Scan memory",                                       cmd_scan_memory},
		{"xx",     "Scan multiple memory",                              cmd_scan_memory_n},
		{"xp",     "Scan physical memory",                              cmd_scan_physical_memory},
		{"xxp",    "Scan multiple physical memory",                     cmd_scan_physical_memory_n},
		{"w",      "add_watchpoint",                                    cmd_add_watch},
		{"d",      "delete_watchpoint",                                 cmd_delete_watch},
		{"attach", "attach difftest",                                   cmd_attach_difftest},
		{"detach", "detach difftest",                                   cmd_detach_difftest},
		{"save",   "save image",                                        cmd_save_image},
		{"load",   "load image",                                        cmd_load_image},
		{"r",      "restart image",                                     cmd_restart},
		/* TODO: Add more commands */
};

void image_save(const char *filename);

void image_load(const char *filename);

static int cmd_load_image(char *args) {
	char *filename = strtok(nullptr, " ");
	image_load(filename);
	return 0;
}

static int cmd_save_image(char *args) {
	char *filename = strtok(nullptr, " ");
	image_save(filename);
	return 0;
}


static int cmd_attach_difftest(char *args) {
	difftest_recover();
	g_diff_test_enabled = true;
	return 0;
}

static int cmd_detach_difftest(char *args) {
	g_diff_test_enabled = false;
	return 0;
}


static int cmd_delete_watch(char *args) {
	if (args == nullptr) {
		printf("please specify expr!\n");
		return 0;
	}
	int id = atoi(args);
	int count = g_watch_point_pool.erase(id);
	if (count) {
		printf("Watchpoint %d deleted\n", id);
	} else {
		printf("Watchpoint %d not exists\n", id);
	}
	return 0;
}

static int cmd_add_watch(char *args) {
	if (args == nullptr) {
		printf("please specify expr!\n");
		return 0;
	}
	auto t = compile_expr(args);
	if (t == nullptr) {
		printf("Invalid EXPR\n");
		return 0;
	}
	int id = g_watch_count++;
	g_watch_point_pool.try_emplace(id, args, std::move(t));
	printf("Added to watchpoint %d\n", id);
	return 0;
}

static void info_watchpoint() {
	printf("%-8s%-16s%-s\n", "id", "value", "expr");
	for (auto&[id, wp]: g_watch_point_pool) {
		printf("%-8d%-16d%-s\n", id, wp.get_value(), wp.get_expr_str().c_str());
	}
}

static int cmd_eval(char *args) {
	if (args == 0) {
		printf("please specify expr!\n");
		return 0;
	}
	auto t = compile_expr(args);
	if (!t) {
		printf("invalid expr\n");
		return 0;
	}
	auto res = t->eval();
	printf("%d[0x%08x]\n", res, res);
	return 0;
}

constexpr int NR_CMD = (sizeof(cmd_table) / sizeof(cmd_table[0]));

static void scan_memory_kernel(char *args, int n, bool physical) {
	auto expr = compile_expr(args);
	int addr_v = expr->eval();
	uint32_t addr_base [[maybe_unused]] = addr_v;
	for (int i = 0; i < n; ++i) {
		auto addr = addr_base + 4 * i;
		if (i % 4 == 0) {
			printf("0x%08x: ", addr);
		}
		printf("0x%08x    ", physical ? paddr_read(addr, 4) : vaddr_read(addr, 4));
		if (i % 4 == 3) {
			printf("\n");
		}
	}
}
static int cmd_scan_memory(char *args) {
	scan_memory_kernel(args, 1, false);
	return 0;
}

static int cmd_scan_memory_n(char *args) {
	char *arg = strtok(nullptr, " ");
	int N = atoi(arg);
	args = arg + strlen(arg) + 1;
	scan_memory_kernel(args, N, false);
	return 0;
}

static int cmd_scan_physical_memory(char *args) {
	scan_memory_kernel(args, 1, true);
	return 0;
}

static int cmd_scan_physical_memory_n(char *args) {
	char *arg = strtok(nullptr, " ");
	int N = atoi(arg);
	args = arg + strlen(arg) + 1;
	scan_memory_kernel(args, N, true);
	return 0;
}


void info_register() {
	auto show = [](const char* name, rtlreg_t value){
		printf("%-8s0x%08x%16d\n", name, value, value);
	};
	for (int i = 0; i < 8; ++i) {
		auto value = cpu.gpr[i]._32;
//		printf("%-8s0x%08x%16d\n", regsl[i], value, value);
		show(regsl[i], value);
	}
	show("eip", cpu.eip);
	show("CR0", cpu.cr0.val);
	show("CR3", cpu.cr3.val);
	using namespace EFLAGS;
	printf("ZF=%d, SF=%d, CF=%d, OF=%d, IF=%d\n", get_ZF(cpu.eflags), get_SF(cpu.eflags), get_CF(cpu.eflags),
			get_OF(cpu.eflags), get_IF(cpu.eflags));
	
}

static int cmd_info(char *args) {
	char *arg = strtok(nullptr, " ");
	if (arg == nullptr) {
		printf("ERROR: Expected 1 Arg(s) \n");
		return 0;
	}
	if (arg == string("r")) {
		info_register();
	} else if (arg == string("w")) {
		info_watchpoint();
	} else {
		printf("Unknown info of %s \n", arg);
	}
	return 0;
}

static int cmd_step_into(char *args) {
	char *arg = strtok(nullptr, " ");
	int step_count;
	if (arg == nullptr) {
		step_count = 1;
	} else {
		step_count = atoi(arg);
	}
	
	cpu_exec(step_count);
	return 0;
}


static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(nullptr, " ");
	int i;
	
	if (arg == nullptr) {
		/* no argument given */
		for (i = 0; i < NR_CMD; i++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	} else {
		for (i = 0; i < NR_CMD; i++) {
			if (strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop(int is_batch_mode) {
	if (is_batch_mode) {
		cmd_c(nullptr);
		return;
	}
	
	for (char *str; (str = rl_gets()) != nullptr;) {
		char *str_end = str + strlen(str);
		
		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if (cmd == nullptr) { continue; }
		
		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if (args >= str_end) {
			args = nullptr;
		}

#ifdef HAS_IOE
		sdl_clear_event_queue();
#endif
		
		int i;
		for (i = 0; i < NR_CMD; i++) {
			if (strcmp(cmd, cmd_table[i].name) == 0) {
				if (cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}
		
		if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
