#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern bool verbose;

static inline void halt(void) {
    fprintf(stderr, "machine halted\n");
    exit(10);
}

#define printfv(fmt, ...) \
    if (verbose) printf(fmt, ##__VA_ARGS__)

#define panic(fmt, ...) ({ \
    fprintf(stderr, "PANIC: " fmt, ##__VA_ARGS__); \
    halt(); \
})

void run_machine(unsigned char *ops, unsigned int max_pc);

#endif
