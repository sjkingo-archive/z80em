#ifndef INSTS_H
#define INSTS_H

struct z80_instruction {
    unsigned short opcode;
    char *name;
    unsigned char cycles;
};

enum z80_opcodes {
    OP_NOP=0x0,

    /* load immediates */
    OP_LD_A_N=0x3e,
    OP_LD_B_N=0x06,
    OP_LD_C_N=0x0e,
    OP_LD_D_N=0x16,
    OP_LD_E_N=0x1e,
    OP_LD_H_N=0x26,
    OP_LD_L_N=0x2e,

    OP_HALT=0x76,
};

struct z80_instruction insts[] = {
    /* { opcode, "name", cycles }, */
    { OP_NOP, "nop", 1 },

    /* load immediates */
    { OP_LD_A_N, "ld A,n", 2 },
    { OP_LD_B_N, "ld B,n", 2 },
    { OP_LD_C_N, "ld C,n", 2 },
    { OP_LD_D_N, "ld D,n", 2 },
    { OP_LD_E_N, "ld E,n", 2 },
    { OP_LD_H_N, "ld H,n", 2 },
    { OP_LD_L_N, "ld L,n", 2 },

    { OP_HALT, "halt", 1 },

    /* always leave this last */
    { 0x0, NULL, 0 },
};

static inline struct z80_instruction *find_opcode(unsigned char opcode) {
    unsigned int i = 0;
    while (insts[i].name != NULL) {
        if (insts[i].opcode == opcode) {
            return &insts[i];
        }
        i++;
    }
    return NULL;
}

#endif
