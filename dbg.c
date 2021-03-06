#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "dbg.h"
#include "cpu.h"
#include "emulator.h"
#include "insts.h"

/* state of the debugger */
struct dbg_info *dbg_state = NULL;

/* prototype due to static ordering */
static void dbg_sigint(int);

static void setup_sigint(void) {
    struct sigaction sa;
    sa.sa_handler = dbg_sigint;
    sa.sa_flags = SA_RESETHAND;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction (setup)");
    }
}

static void dbg_sigint(int signum __attribute__((unused))) {
    printf("\nCaught interrupt, breaking\n");
    dbg_break();
    setup_sigint();
}

char *disass_opcode(unsigned short offset) {
    char *i = malloc(1024);

    unsigned char opcode = cpu->code[offset];
    struct z80_instruction *inst = find_opcode(opcode);
    if (inst == NULL) {
        sprintf(i, "??\t\t%04x", opcode);
    } else {
        unsigned int x = 0;
        sprintf(i, "%s ", inst->name);

        /* append the static args */
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

        /* append the hex dump of the instruction */
        x = 0;
        strcat(i, "\t\t");
        while (x < inst->cycles) {
            char b[20];
            sprintf(b, "%04x ", cpu->code[offset+x]);
            strcat(i, b);
            x++;
        }
    }

    char *r = malloc(1024);
    sprintf(r, "%04x\t\t%s", offset, i);
    free(i);
    return r;
}

static void wait_for_input(void) {
    while (dbg_enabled()) {
        char *line, *b, *cmd, *arg;
        char **args;
        unsigned int nargs = 0;

        line = readline("(dbg) ");
        if (line == NULL)
            exit(0);
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

static void internal_break(void) {
    static unsigned int break_n = 1;
    printf("\n#%03d\tpc=%04x\t(not yet executed)\n", break_n++, cpu->regs.pc);
    wait_for_input();
    dbg_enable();
}

void dbg_break(void) {
    if (dbg_state->break_func == NULL)
        internal_break();
    else
        dbg_state->break_func();
}

void dbg_write_history(int exit_status __attribute__((unused)), 
        void *history_filename) {
    if (write_history((char *)history_filename) < 0) {
        perror("could not write history file");
    }
}

void dbg_init(bool enabled, char *history_filename, void (*callback)(void)) {
    dbg_state = malloc(sizeof(*dbg_state));
    memset(dbg_state, 0, sizeof(*dbg_state));
    dbg_state->enabled = enabled;
    dbg_state->cont_possible = true;
    dbg_state->break_func = callback;

    if (enabled) {
        dbg_state->break_func = callback;

        setup_sigint();

        using_history();
        if (history_filename != NULL) {
            if (read_history(history_filename) < 0) {
                perror("could not load history file");
            }
            on_exit(dbg_write_history, history_filename);
        }
    }
}

void disassemble_objfile(unsigned int max_insts) {
    unsigned short offset = 0;
    unsigned int i = 0;
    printf("assembler dump from start of objfile:\n");

    while (offset < cpu->max_pc) {
        /* pc */
        if (offset == cpu->regs.pc)
            printf("=> ");
        else
            printf("   ");

        /* print the disassembled opcode */
        char *l = disass_opcode(offset);
        printf("%s\n", l);
        free(l);

        /* work out how far to move ahead for the next instruction */
        struct z80_instruction *inst = find_opcode(cpu->code[offset]);
        if (inst == NULL) {
            /* assume cycles = 1 */
            offset++;
        } else {
            offset += inst->cycles;
        }

        i++;
        if (max_insts != 0 && i > max_insts) 
            break;
    }

    if (max_insts != 0)
        printf("truncated assembler dump to %d instructions\n", max_insts);
    else
        printf("end of assembler dump\n");
}
