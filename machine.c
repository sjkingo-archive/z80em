#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "dbg.h"
#include "cpu.h"
#include "insts.h"
#include "emulator.h"

#define mark_pc() \
    if (pc_at != -1) { \
        for (unsigned short p = 0; p < pc_at; p++) \
            printf(" "); \
        printf("^ pc\n"); \
        pc_at = -1; \
    }

void dump_objfile(void) {
    unsigned short i;
    unsigned int this_row = 0;
    int pc_at = -1;

    for (i = 0; i <= cpu->max_pc; i++) {
        unsigned int this_print = printf("%04x", cpu->code[i]);
        this_row += this_print;

        if (i == cpu->regs.pc)
            pc_at = this_row - this_print;

        if (i != 0 && i % 4 == 0) {
            printf("\n");
            this_row = 0;
            mark_pc();
        } else {
            printf(" ");
            this_row++;
        }
    }

    if (i % 4 != 0) {
        printf("\n");
        mark_pc();
    }
}

void run_machine(unsigned char *ops, unsigned int max_pc) {
    printfv("starting emulation\n");
    printfv("executable code loaded at %p\n\n", ops);

    cpu->max_pc = max_pc;
    cpu->code = ops;

    if (enable_dbg)
        dbg_break();

    while (cpu->regs.pc < max_pc) {
        unsigned char opcode = ops[cpu->regs.pc];
        printfv("pc=%d, opcode=0x%x\n", cpu->regs.pc, opcode);

        struct z80_instruction *inst = find_opcode(opcode);
        if (inst == NULL) {
            if (enable_dbg) {
                printf("Error: unknown opcode 0x%x\n", opcode);
                dbg_cont_possible = false;
                dbg_break();
                panic("dbg exited when emulation cannot continue\n");
            } else {
                panic("unknown opcode 0x%x\n", opcode);
            }
        }
        printfv("  %s\n", inst->name);

        /* the meat of the emulator - instruction emulation */
        switch (opcode) {
            case OP_NOP:
                break;

            /* load immediates */
            case OP_LD_A_N:
            case OP_LD_B_N:
            case OP_LD_C_N:
            case OP_LD_D_N:
            case OP_LD_E_N:
            case OP_LD_H_N:
            case OP_LD_L_N: {
                unsigned char n = ops[cpu->regs.pc+1];
                switch (opcode) {
                    case OP_LD_A_N:
                        set_reg(REG_A, n);
                        break;
                    case OP_LD_B_N:
                        set_reg(REG_B, n);
                        break;
                    case OP_LD_C_N:
                        set_reg(REG_C, n);
                        break;
                    case OP_LD_D_N:
                        set_reg(REG_D, n);
                        break;
                    case OP_LD_E_N:
                        set_reg(REG_E, n);
                        break;
                    case OP_LD_H_N:
                        set_reg(REG_H, n);
                        break;
                    case OP_LD_L_N:
                        set_reg(REG_L, n);
                        break;
                }
                break;
            }

            /* load r' into r */
            case OP_LD_A_A:
                /* nop */
                printfv("what possible use is ld A,A ??\n");
                break;
            case OP_LD_A_B:
                set_reg(REG_A, get_reg(REG_B));
                break;
            case OP_LD_A_C:
                set_reg(REG_A, get_reg(REG_C));
                break;
            case OP_LD_A_D:
                set_reg(REG_A, get_reg(REG_D));
                break;
            case OP_LD_A_E:
                set_reg(REG_A, get_reg(REG_E));
                break;
            case OP_LD_A_H:
                set_reg(REG_A, get_reg(REG_H));
                break;
            case OP_LD_A_L:
                set_reg(REG_A, get_reg(REG_L));
                break;

            case OP_HALT:
                /* halt until interrupt */
                for (;;); /* nop */
                break;

            default:
                if (enable_dbg) {
                    printf("Error: unhandled opcode 0x%x (%s)\n", opcode, inst->name);
                    dbg_break();
                } else {
                    panic("unhandled opcode %s (0x%x)\n", inst->name, opcode);
                }
        }

        cpu->regs.pc += inst->cycles;
    }

    printfv("\nfinished emulation\n");
}
