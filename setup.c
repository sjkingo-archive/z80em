#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "cpu.h"
#include "dbg.h"
#include "emulator.h"

bool verbose = false;

struct emulator_state *init_emulator(char *filename, bool enable_dbg, 
        char *dbg_history, void (*dbg_callback)(void)) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Unable to open object file");
        exit(3);
    }

    struct stat s;
    if (fstat(fd, &s) < 0) {
        perror("Unable to stat object file");
        exit(3);
    }

    void *ops = mmap(NULL, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ops == NULL) {
        perror("Unable to map object file into memory");
        exit(3);
    }

    init_cpu_state();

    /* set up our emulator state */
    struct emulator_state *state = malloc(sizeof(*state));
    state->ops = (char *)ops;
    state->size = s.st_size;
    state->_fd = fd;

    /* init the debugger */
    dbg_init(enable_dbg, dbg_history, dbg_callback);

    return state;
}

void deinit_emulator(struct emulator_state *state) {
    munmap((void *)state->ops, state->size);
    close(state->_fd);
    free(state);
}
