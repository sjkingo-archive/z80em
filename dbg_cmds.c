#include <stdlib.h>

#include "debugger.h"

static void cmd_show(char **args) {
}

struct debugger_command dbg_cmds[] = {
    { "show", &cmd_show, "regs" },

    { NULL, NULL, NULL }, /* sentinel entry; don't remove */
};

