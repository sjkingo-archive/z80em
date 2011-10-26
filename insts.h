#ifndef INSTS_H
#define INSTS_H

struct z80_instruction {
    unsigned short opcode;
    char *name;
    unsigned char cycles;
    unsigned short n_vargs; /* number of variable args */
    char *sargs[10]; /* array of static args terminated by NULL */
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
    /* { opcode, "name", {"sarg1", .., NULL}, n_vargs, cycles }, */
    { OP_NOP, "nop", 1, 0, {NULL} },

    /* load immediates */
    { OP_LD_A_N, "ld", 2, 1, {"A", NULL} },
    { OP_LD_B_N, "ld", 2, 1, {"B", NULL} },
    { OP_LD_C_N, "ld", 2, 1, {"C", NULL} },
    { OP_LD_D_N, "ld", 2, 1, {"D", NULL} },
    { OP_LD_E_N, "ld", 2, 1, {"E", NULL} },
    { OP_LD_H_N, "ld", 2, 1, {"H", NULL} },
    { OP_LD_L_N, "ld", 2, 1, {"L", NULL} },

    /* load register r' into r */
    { OP_LD_A_B, "ld", 1, 0, {"A", "B", NULL} },
    { OP_LD_A_C, "ld", 1, 0, {"A", "C", NULL} },
    { OP_LD_A_D, "ld", 1, 0, {"A", "D", NULL} },
    { OP_LD_A_E, "ld", 1, 0, {"A", "E", NULL} },
    { OP_LD_A_H, "ld", 1, 0, {"A", "H", NULL} },
    { OP_LD_A_L, "ld", 1, 0, {"A", "L", NULL} },
    { OP_LD_A_A, "ld", 1, 0, {"A", "A", NULL} },

    { OP_HALT, "halt", 1, 0, {NULL} },

    /* always leave this last */
    { 0x0, NULL, 0, 0, {NULL} },
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
