#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

static void wait_for_input(void) {
    char buf[1024];
    do {
        printf("(dbg) ");
        fflush(stdout);
        
    } while (fgets(buf, sizeof(buf), stdin) != NULL);
    exit(0);
}

void debugger_break(void) {
    printf("Breaking to debugger\n");
    printf("pc=%d\n", cpu->regs.pc);
    wait_for_input();
}
