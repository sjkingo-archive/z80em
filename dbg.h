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

/* write history to the given filename
 * note the argument is a void * for on_exit()'s sake, simply cast to char *
 */
void dbg_write_history(int exit_status, void *history_filename);

/* init the debugger and optionally load history if history_filename is not NULL */
void dbg_init(char *history_filename);

#endif
