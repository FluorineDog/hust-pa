#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "wheel/string_tools.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char *rl_gets() {
    static char *line_read = NULL;

    if (line_read) {
        free(line_read);
        line_read = NULL;
    }

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

static int cmd_help(char *args);

static int cmd_step_into(char *args);

static int cmd_info(char *args);

static int cmd_eval(char *args);

static struct {
    const char *name;
    const char *description;

    int (*handler)(char *);
} cmd_table[] = {
        {"help", "Display informations about all supported commands", cmd_help},
        {"c",    "Continue the execution of the program",             cmd_c},
        {"q",    "Exit NEMU",                                         cmd_q},
        {"si",   "Step [N] instruction(s)",                           cmd_step_into},
        {"info", "Provide information of (r)egister/(w)atchpoint",    cmd_info},
        {"p",    "Eval expression",                                   cmd_eval},
        /* TODO: Add more commands */

};

static int cmd_eval(char *args){
    if(args == 0){
        printf("please specify expr");
    }
    auto t = compile_expr(args);
    if(!t){
        printf("invalid expr\n");
        return 0;
    }
    auto res = t->eval();
    printf("%d\n", res);
    return 0;
}

constexpr int NR_CMD = (sizeof(cmd_table) / sizeof(cmd_table[0]));




static void info_register() {
    const char *names[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eip",};

    for (int i = 0; i < 8; ++i) {
        auto value = cpu.gpr[i]._32;
        printf("%-8s0x%08x%16d\n", names[i], value, value);
    }
}

static void info_watchpoint() {
    // TODO
}

static int cmd_info(char *args) {
    char *arg = strtok(NULL, " ");
    if (arg == NULL) {
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
    char *arg = strtok(NULL, " ");
    int step_count;
    if (arg == NULL) {
        step_count = 1;
    } else {
        step_count = atoi(arg);
    }

    cpu_exec(step_count);
    return 0;
}


static int cmd_help(char *args) {
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL) {
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
        cmd_c(NULL);
        return;
    }

    for (char *str; (str = rl_gets()) != NULL;) {
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL) { continue; }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL;
        }

#ifdef HAS_IOE
        extern void sdl_clear_event_queue(void);
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
