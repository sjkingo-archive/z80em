#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "dbg.h"
#include "cpu.h"
#include "emulator.h"

bool dbg_cont_possible = true;
bool dbg_ss = false;

static void wait_for_input(void) {
    while (enable_dbg) {
        char *line, *b, *cmd, *arg;
        char **args;
        unsigned int nargs = 0;

        line = readline("(dbg) ");
        add_history(line);
        b = line;

        cmd = strsep(&b, " ");
        if (cmd == NULL || strlen(cmd) == 0) {
            free(line);
            continue;
        }

        args = malloc(sizeof(char *));
        while ((arg = strsep(&b, " ")) != NULL) {
            args[nargs] = malloc(strlen(arg)+1);
            strcpy(args[nargs], arg);
            args = realloc(args, (++nargs)*sizeof(char *));
        }
        args[nargs] = NULL;

        bool found = false;
        unsigned int i = 0;
        while (dbg_cmds[i].cmd != NULL) {
            if (strcmp(dbg_cmds[i].cmd, cmd) == 0) {
                dbg_cmds[i].func(args);
                found = true;
                break;
            }
            i++;
        }

        if (!found)
            printf("dbg: unknown command %s\n", cmd);

        i = 0;
        while (args[i] != NULL) {
            free(args[i]);
            i++;
        }
        free(args);
        free(line);
    }
}

void dbg_break(void) {
    static unsigned int break_n = 0;
    printf("\n#%d pc=%d\n", break_n++, cpu->regs.pc);
    wait_for_input();
    enable_dbg = true;
}

void dbg_write_history(int exit_status __attribute__((unused)), 
        void *history_filename) {
    if (write_history((char *)history_filename) < 0) {
        perror("could not write history file");
    }
}

void dbg_init(char *history_filename) {
    using_history();
    if (history_filename != NULL) {
        if (read_history(history_filename) < 0) {
            perror("could not load history file");
        }
        on_exit(dbg_write_history, history_filename);
    }
}
