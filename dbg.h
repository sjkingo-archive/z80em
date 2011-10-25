#ifndef DBG_H
#define DBG_H

#include <stdbool.h>

struct dbg_cmd_entry {
    char *cmd;
    void (*func)(char **);
    char *help;
};
extern struct dbg_cmd_entry dbg_cmds[];

extern bool dbg_cont_possible;
extern bool dbg_ss;

void dbg_break(void);

#endif
