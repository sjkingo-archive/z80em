#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "debugger.h"
#include "cpu.h"
#include "insts.h"
#include "emulator.h"

void run_machine(unsigned char *ops, unsigned int max_pc) {
    printfv("starting emulation\n\n");

    if (enable_debugger)
        debugger_break();

    while (cpu->regs.pc < max_pc) {
        unsigned char opcode = ops[cpu->regs.pc];
        printfv("pc=%d, opcode=0x%x\n", cpu->regs.pc, opcode);

        struct z80_instruction *inst = find_opcode(opcode);
        if (inst == NULL) {
            if (enable_debugger) {
                printf("Error: unknown opcode 0x%x\n", opcode);
                debugger_cont_possible = false;
                debugger_break();
                panic("debugger exited when emulation cannot continue\n");
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

            case OP_HALT:
                /* halt until interrupt */
                for (;;); /* nop */
                break;

            default:
                if (enable_debugger) {
                    printf("Error: unhandled opcode 0x%x (%s)\n", opcode, inst->name);
                    debugger_break();
                } else {
                    panic("unhandled opcode %s (0x%x)\n", inst->name, opcode);
                }
        }

        cpu->regs.pc += inst->cycles;
    }

    printfv("\nfinished emulation\n");
}
