#ifndef INSTS_H
#define INSTS_H

struct z80_instruction {
    unsigned short opcode;
    char *name;
    char *args;
    unsigned short n_args;
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

    /* load register r' into r */
    OP_LD_A_B=0x78,
    OP_LD_A_C=0x79,
    OP_LD_A_D=0x7A,
    OP_LD_A_E=0x7B,
    OP_LD_A_H=0x7C,
    OP_LD_A_L=0x7D,
    OP_LD_A_A=0x7F,

    OP_HALT=0x76,
};

static struct z80_instruction insts[] = {
    /* { opcode, "name", "args", n_args, cycles }, */
    { OP_NOP, "nop", NULL, 0, 1 },

    /* load immediates */
    { OP_LD_A_N, "ld", "A,n", 1, 2 },
    { OP_LD_B_N, "ld", "B,n", 1, 2 },
    { OP_LD_C_N, "ld", "C,n", 1, 2 },
    { OP_LD_D_N, "ld", "D,n", 1, 2 },
    { OP_LD_E_N, "ld", "E,n", 1, 2 },
    { OP_LD_H_N, "ld", "H,n", 1, 2 },
    { OP_LD_L_N, "ld", "L,n", 1, 2 },

    /* load register r' into r */
    { OP_LD_A_B, "ld", "A,B", 0, 1 },
    { OP_LD_A_C, "ld", "A,C", 0, 1 },
    { OP_LD_A_D, "ld", "A,D", 0, 1 },
    { OP_LD_A_E, "ld", "A,E", 0, 1 },
    { OP_LD_A_H, "ld", "A,H", 0, 1 },
    { OP_LD_A_L, "ld", "A,L", 0, 1 },
    { OP_LD_A_A, "ld", "A,A", 0, 1 },

    { OP_HALT, "halt", NULL, 0, 1 },

    /* always leave this last */
    { 0x0, NULL, NULL, 0, 0 },
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
