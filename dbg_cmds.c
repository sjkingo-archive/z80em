#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "dbg.h"
#include "emulator.h"

static void cmd_cont(char **args __attribute__((unused))) {
    if (dbg_cont_possible)
        enable_dbg = false;
    else
        printf("dbg: cannot continue due to previous error\n");
}

static void cmd_show(char **args) {
    if (args[0] == NULL) {
        printf("show [option]\n");
        printf("     regs\tShow the status of the CPU registers\n");
        return;
    }

    if (strcmp(args[0], "regs") == 0) {
        printf("pc=%d\n", cpu->regs.pc);
        printf("A=%x\tB=%x\tC=%x\tD=%x\n", cpu->regs.a, cpu->regs.b, cpu->regs.c,
                cpu->regs.d);
        printf("E=%x\tH=%x\tL=%x\n", cpu->regs.e, cpu->regs.h, cpu->regs.l);
        return;
    }
}

static void cmd_set(char **args) {
    if (args[0] == NULL || args[1] == NULL) {
        printf("set A|B|C|D|E|H|L|pc n\n");
        return;
    }

    int n = atoi(args[1]);
    
    if (strcmp(args[0], "pc") == 0) {
        set_pc(n);
    } else {
        switch (args[0][0]) {
            case 'A':
                set_reg(REG_A, n);
                break;
            case 'B':
                set_reg(REG_B, n);
                break;
            case 'C':
                set_reg(REG_C, n);
                break;
            case 'D':
                set_reg(REG_D, n);
                break;
            case 'E':
                set_reg(REG_E, n);
                break;
            case 'H':
                set_reg(REG_H, n);
                break;
            case 'L':
                set_reg(REG_L, n);
                break;
            default:
                printf("unknown register %c\n", args[0][0]);
                return;
        }
    }
}

struct dbg_cmd_entry dbg_cmds[] = {
    { "c", &cmd_cont, "Continue execution." },
    { "show", &cmd_show, "Show various information about the emulation." },
    { "set", &cmd_set, "Set various registers." },

    { NULL, NULL, NULL }, /* sentinel entry; don't remove */
};

