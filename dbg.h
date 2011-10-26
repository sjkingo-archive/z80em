#ifndef DBG_H
#define DBG_H

#include <stdbool.h>

struct dbg_cmd_entry {
    char *cmd;
    void (*func)(char **);
    char *help;
};
extern struct dbg_cmd_entry dbg_cmds[];

extern bool enable_dbg;
extern bool dbg_cont_possible;
extern bool dbg_ss;

/* given an offset into the executable code, attempt to disassemble it
 * and return a string ready for printing (no newline). note the caller
 * must free this string.
 */
char *disass_opcode(unsigned short offset);

void dbg_break(void);

/* write history to the given filename
 * note the argument is a void * for on_exit()'s sake, simply cast to char *
 */
void dbg_write_history(int exit_status, void *history_filename);

/* init the debugger and optionally load history if history_filename is not NULL */
void dbg_init(char *history_filename);

/* print disassembly of objfile, up to max_insts or full output if 0 */
void disassemble_objfile(unsigned int max_insts);

#endif
