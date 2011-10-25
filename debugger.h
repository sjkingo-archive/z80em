#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdbool.h>

struct debugger_command {
    char *cmd;
    void (*func)(char **);
    char *help;
};
extern struct debugger_command dbg_cmds[];

extern bool debugger_cont_possible;

void debugger_break(void);

#endif
