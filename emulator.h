#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern bool verbose, enable_dbg;

static inline void halt(void) {
    fprintf(stderr, "machine halted\n");
    exit(10);
}

#define printfv(fmt, ...) \
    if (verbose) printf(fmt, ##__VA_ARGS__)

#define panic(fmt, ...) ({ \
    fprintf(stderr, "PANIC: %s(): " fmt, __func__, ##__VA_ARGS__); \
    halt(); \
})

/* hexdump the objfile with program counter marker */
void dump_objfile(FILE *out);

void run_machine(unsigned char *ops, unsigned int max_pc);

#endif
