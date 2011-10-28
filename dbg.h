#ifndef DBG_H
#define DBG_H

#include <stdbool.h>

struct dbg_cmd_entry {
    char *cmd;
    void (*func)(char **);
    char *help;
};
extern struct dbg_cmd_entry dbg_cmds[];

struct dbg_info {
    bool enabled; /* whether the debugger is enabled */
    bool cont_possible; /* is it possible to continue execution */
    bool single_step; /* single step execution? */

    void (*break_func)(void); /* callback for breaking into the debugger */
};
extern struct dbg_info *dbg_state;

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
void dbg_init(bool enabled, char *history_filename, 
        void (*break_func)(void));

/* print disassembly of objfile, up to max_insts or full output if 0 */
void disassemble_objfile(unsigned int max_insts);

/* returns true if debugger is enabled */
static inline bool dbg_enabled(void) {
    return dbg_state->enabled;
}

/* enables the debugger */
static inline void dbg_enable(void) {
    dbg_state->enabled = true;
}

/* disables the debugger */
static inline void dbg_disable(void) {
    dbg_state->enabled = false;
}

/* returns true if continuation is possible */
static inline bool dbg_cont_possible(void) {
    return dbg_state->cont_possible;
}

/* disables continuation of the debugger */
static inline void dbg_disable_cont(void) {
    dbg_state->cont_possible = false;
}

/* returns true if single stepping is enabled */
static inline bool dbg_should_ss(void) {
    return dbg_state->single_step;
}

/* toggles single stepping */
static inline void dbg_enable_ss(void) {
    dbg_state->single_step = true;
}
static inline void dbg_disable_ss(void) {
    dbg_state->single_step = false;
}

#endif
