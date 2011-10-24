#ifndef MACHINE_H
#define MACHINE_H

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

struct z80_cpu_state {
    unsigned short pc; /* program counter */
    struct z80_status_reg status;
};

void run_machine(unsigned char *ops, unsigned int max_pc);

#endif
