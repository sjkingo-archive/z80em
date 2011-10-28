#ifndef CPU_H
#define CPU_H

#include <stdio.h>

#define STACK_SIZE 256
#define RAM_SIZE 1024

/* page 76 */
struct z80_status_reg {
    unsigned char c; /* carry */
    unsigned char n1; /* add/subtract, bit 1 */
    unsigned char pv; /* parity/overflow */
    unsigned char _unused1;
    unsigned char n2; /* add/subtract, bit 4 */
    unsigned char _unused2;
    unsigned char z; /* half carry */
    unsigned char s; /* sign flag */
} __attribute__((packed));

enum z80_registers {
    REG_A=0xF,
    REG_B=0x0,
    REG_C=0x1,
    REG_D=0x2,
    REG_E=0x3,
    REG_H=0x4,
    REG_L=0x5,
    REG_HL
};

struct z80_register_state {
    unsigned short pc; /* program counter */
    unsigned short sp; /* stack pointer, max = &stack+STACK_SIZE */

    /* general purpose registers */
    char a;
    char b;
    char c;
    char d;
    char e;
    char h;
    char l;
};

struct z80_cpu_state {
    struct z80_status_reg status;
    struct z80_register_state regs;

    unsigned short max_pc; /* max program counter value */
    char *code; /* executable code */

    char mem[RAM_SIZE]; /* writeable memory */
    char stack[STACK_SIZE]; /* the stack, grows downwards into mem */
};

extern struct z80_cpu_state *cpu;

/* initialise the cpu core - this should only be called once */
void init_cpu_state(void);

/* sets or gets the given register to n/returns (immediate load) */
void set_reg(unsigned char reg, char n);
char get_reg(unsigned char reg);

/* get the single character name of the register given by reg */
unsigned char get_reg_name(unsigned char reg);

/* sets the program counter to the given value, and returns the old one */
unsigned short set_pc(unsigned short new_pc);

/* computes the offset and sets the stack pointer to the given value, and returns the old one */
unsigned short set_sp(unsigned short new_sp);

/* print the current CPU registers to the file stream out */
void print_regs(FILE *out);

#endif
