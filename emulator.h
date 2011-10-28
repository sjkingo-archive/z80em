#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <stdio.h>

struct emulator_state {
    char *ops;
    unsigned int size;

    /* internal variables; do not use */
    int _fd;
};

extern bool verbose;

#define printfv(fmt, ...) \
    if (verbose) printf(fmt, ##__VA_ARGS__)

/* abnormally halt the machine and drop to the debugger if enabled */
void halt(void);

/* write error message and halt the machine */
#define panic(fmt, ...) ({ \
    fprintf(stderr, "\nPANIC: %s(): " fmt, __func__, ##__VA_ARGS__); \
    halt(); \
})

/* hexdump the objfile with program counter marker */
void dump_objfile(FILE *out);

/* this must be called before run_machine() */
struct emulator_state *init_emulator(char *filename, bool enable_dbg, 
        char *dbg_history, void (*dbg_callback)(void));

/* called to clean up when emulation is finished */
void deinit_emulator(struct emulator_state *state);

/* run the emulation */
void run_machine(struct emulator_state *state);

#endif
