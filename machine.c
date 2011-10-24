#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "insts.h"
#include "emulator.h"

void run_machine(unsigned char *ops, unsigned int max_pc) {
    printfv("starting emulation\n\n");

    while (cpu->regs.pc < max_pc) {
        unsigned char opcode = ops[cpu->regs.pc];
        printfv("pc=%d, opcode=0x%x\n", cpu->regs.pc, opcode);

        struct z80_instruction *inst = find_opcode(opcode);
        if (inst == NULL) {
            panic("invalid opcode 0x%x\n", opcode);
        }
        printfv("  %s\n", inst->name);

        switch (opcode) {
            case OP_NOP:
                break;

            case OP_LD_B_N:
            case OP_LD_H_N: {
                /* immediate load n into reg X */
                unsigned char n = ops[cpu->regs.pc+1];
                switch (opcode) {
                    case OP_LD_B_N:
                        set_reg(REG_B, n);
                        break;
                    case OP_LD_H_N:
                        set_reg(REG_H, n);
                        break;
                }
                break;
            }

            default:
                panic("unhandled opcode %s (0x%x)\n", inst->name, opcode);
        }
        cpu->regs.pc += inst->cycles;
    }

    printfv("\nfinished emulation\n");
}
