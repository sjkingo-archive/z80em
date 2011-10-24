#ifndef INSTS_H
#define INSTS_H

struct z80_instruction {
    unsigned short opcode;
    char *name;
    unsigned char cycles;
};

enum z80_opcodes {
    OP_NOP=0x0,
    OP_LD_B_N=0x6,
    OP_LD_H_N=0x26,
};

struct z80_instruction insts[] = {
    /* { opcode, "name", cycles }, */
    { OP_NOP, "nop", 1 },

    /* load immediates */
    { OP_LD_B_N, "ld B,n", 2 },
    { OP_LD_H_N, "ld H,n", 2 },

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
