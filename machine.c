#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "insts.h"
#include "machine.h"

extern bool verbose;

#define printfv(fmt, ...) \
    if (verbose) printf("%s(): " fmt, __func__, ##__VA_ARGS__)

static void halt(void) {
    fprintf(stderr, "machine halted\n");
    exit(10);
}

void run_machine(unsigned char *ops, unsigned int max_pc) {
    struct z80_cpu_state cpu;
    memset(&cpu, 0, sizeof(cpu));

    printfv("enter\n");
    while (cpu.pc < max_pc) {
        unsigned char opcode = ops[cpu.pc];
        struct z80_instruction *inst = find_opcode(opcode);
        if (inst == NULL) {
            fprintf(stderr, "invalid opcode 0x%x\n", opcode);
            halt();
        }
        printfv("opcode = 0x%x, inst = %s\n", opcode, inst->name);
        switch (opcode) {
            case OP_NOP:
                break;
            case OP_LD_RN: {
                /* immediate load n into r */
                unsigned char n = ops[cpu.pc+1];
                unsigned char r = ops[cpu.pc+2];
                printfv("  %x <- %x\n", r, n);
                break;
            }
        }
        cpu.pc += inst->cycles;
    }
    printfv("exit\n");
}
