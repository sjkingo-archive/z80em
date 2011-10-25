#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "emulator.h"
#include "cpu.h"

struct z80_cpu_state *cpu;

void init_cpu_state(void) {
    static bool init_complete = false;
    if (init_complete)
        panic("already called");

    cpu = malloc(sizeof(*cpu));
    memset(cpu, 0, sizeof(*cpu));
    init_complete = true;
}

void set_reg(unsigned char reg, unsigned char n) {
    switch (reg) {
        case REG_A:
            cpu->regs.a = n;
            break;
        case REG_B:
            cpu->regs.b = n;
            break;
        case REG_C:
            cpu->regs.c = n;
            break;
        case REG_D:
            cpu->regs.d = n;
            break;
        case REG_E:
            cpu->regs.e = n;
            break;
        case REG_H:
            cpu->regs.h = n;
            break;
        case REG_L:
            cpu->regs.l = n;
            break;
        default:
            panic("unknown register %04x\n", reg);
    }
    printfv("  %c (%04x) <- %04x\n", get_reg_name(reg), reg, n);
}

unsigned char get_reg_name(unsigned char reg) {
    switch (reg) {
        case REG_A:
            return 'A';
        case REG_B:
            return 'B';
        case REG_C:
            return 'C';
        case REG_D:
            return 'D';
        case REG_E:
            return 'E';
        case REG_H:
            return 'H';
        case REG_L:
            return 'L';
    }
    panic("unknown register %04x\n", reg);
    return '?';
}

unsigned short set_pc(unsigned short new_pc) {
    if (new_pc > cpu->max_pc)
        panic("pc out of range (%04x > %04x)\n", new_pc, cpu->max_pc);
    unsigned short old_pc = cpu->regs.pc;
    cpu->regs.pc = new_pc;
    printfv("  pc <- %04x\n", new_pc);
    return old_pc;
}

unsigned char get_reg(unsigned char reg) {
    switch (reg) {
        case REG_A:
            return cpu->regs.a;
        case REG_B:
            return cpu->regs.b;
        case REG_C:
            return cpu->regs.c;
        case REG_D:
            return cpu->regs.d;
        case REG_E:
            return cpu->regs.e;
        case REG_H:
            return cpu->regs.h;
        case REG_L:
            return cpu->regs.l;
    }
    panic("unknown register %04x\n", reg);
    return 0;
}
