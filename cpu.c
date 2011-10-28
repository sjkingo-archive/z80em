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

    printfv("ram located between %p-%p\n", cpu->mem, cpu->mem+RAM_SIZE);
    printfv("stack located between %p-%p\n", cpu->stack, cpu->stack+STACK_SIZE);
    set_sp(STACK_SIZE);
    init_complete = true;
}

void set_reg(unsigned char reg, char n) {
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

unsigned short set_sp(unsigned short new_sp) {
    unsigned short old_sp = cpu->regs.sp;
    cpu->regs.sp = new_sp;
    printfv("  sp <- %04x (%p)\n", new_sp, cpu->stack+new_sp);
    return old_sp;
}

char get_reg(unsigned char reg) {
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
        case REG_HL:
            return (get_reg(REG_H) << 8) | get_reg(REG_L);
    }
    panic("unknown register %04x\n", reg);
    return 0;
}

void print_regs(FILE *out) {
    fprintf(out, "pc=%04x\tsp=%04x\n", cpu->regs.pc, cpu->regs.sp);
    fprintf(out, "A=%04x\tB=%04x\tC=%04x\tD=%04x\n", cpu->regs.a, cpu->regs.b, cpu->regs.c,
            cpu->regs.d);
    fprintf(out, "E=%04x\tH=%04x\tL=%04x\n", cpu->regs.e, cpu->regs.h, cpu->regs.l);
    fprintf(out, "HL=%04x\n", get_reg(REG_HL));
}
