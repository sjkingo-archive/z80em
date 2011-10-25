#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "debugger.h"
#include "cpu.h"
#include "emulator.h"

static void wait_for_input(void) {
    char buf[1024];

    for (;;) {
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

void debugger_break(void) {
    printf("Breaking to debugger\n");
    printf("pc=%d\n", cpu->regs.pc);
    wait_for_input();
    enable_debugger = false;
    printf("Exiting debugger\n\n");
}
