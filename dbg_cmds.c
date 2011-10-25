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

struct dbg_cmd_entry dbg_cmds[] = {
    { "c", &cmd_cont, "Continue execution." },
    { "show", &cmd_show, "Show various information about the emulation." },

    { NULL, NULL, NULL }, /* sentinel entry; don't remove */
};

