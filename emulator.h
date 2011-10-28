#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <stdio.h>

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

void run_machine(char *ops, unsigned int max_pc);

#endif
