#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dbg.h"
#include "cpu.h"
#include "emulator.h"

bool dbg_cont_possible = true;
bool dbg_ss = false;

static void wait_for_input(void) {
    char buf[1024];

    while (enable_dbg) {
        char *b, *cmd, *arg;
        char **args;
        unsigned int nargs = 0;

        printf("(dbg) ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }

        b = buf;
        b[strlen(b)-1] = '\0';

        cmd = strsep(&b, " ");
        if (cmd == NULL || strlen(cmd) == 0)
            continue;

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
    }
}

void dbg_break(void) {
    static unsigned int break_n = 0;
    printf("\n#%d pc=%d\n", break_n++, cpu->regs.pc);
    wait_for_input();
    enable_dbg = true;
}
