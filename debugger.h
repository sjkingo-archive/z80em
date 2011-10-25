#ifndef DEBUGGER_H
#define DEBUGGER_H

struct debugger_command {
    char *cmd;
    void (*func)(char **);
    char *help;
};
extern struct debugger_command dbg_cmds[];

void debugger_break(void);

#endif
