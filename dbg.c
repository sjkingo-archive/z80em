#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "dbg.h"
#include "cpu.h"
#include "emulator.h"
#include "insts.h"

bool enable_dbg = false;
bool dbg_cont_possible = true;
bool dbg_ss = false;

char *disass_opcode(unsigned short offset) {
    char *i = malloc(1024);

    unsigned char opcode = cpu->code[offset];
    struct z80_instruction *inst = find_opcode(opcode);
    if (inst == NULL) {
        sprintf(i, "%04x", opcode);
    } else {
        sprintf(i, "%s ", inst->name);

        /* append the static args */
        unsigned int x = 0;
        while (inst->sargs[x] != NULL) {
            strcat(i, inst->sargs[x]);
            if (inst->sargs[x+1] == NULL) {
                if (inst->n_vargs != 0)
                    strcat(i, ",");
            } else {
                strcat(i, ",");
            }
            x++;
        }

        /* append the variable args */
        if (inst->n_vargs != 0) {
            for (x = 1; x <= inst->n_vargs; x++) {
                char b[512];
                sprintf(b, "%d", cpu->code[offset+x]);
                strcat(i, b);
                if (x+1 < inst->n_vargs)
                    strcat(i, ",");
            }
        }
    }

    char *r = malloc(1024);
    sprintf(r, "%04x\t\t%s", offset, i);
    free(i);
    return r;
}

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
            printf("dbg: unknown command %s; try help\n", cmd);

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
    static unsigned int break_n = 1;
    printf("\n#%03d\tpc=%04x\n", break_n++, cpu->regs.pc);
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
