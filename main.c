#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "machine.h"

bool verbose = false;

static inline void print_usage_error(void) {
    fprintf(stderr, "Usage: z80em [-v] objfile\n");
}

static char *handle_args(int argc, char **argv) {
    char *objfile = NULL;

    if (argc <= 1 || argc > 3) {
        print_usage_error();
        exit(1);
    }

    argv++;

    if (strcmp(argv[0], "-h") == 0) {
        print_usage_error();
        exit(0);
    }
    if (strcmp(argv[0], "-v") == 0) {
        verbose = true;
        argv++;
    }

    objfile = argv[0];
    if (objfile == NULL) {
        /* -v given but no filename */
        fprintf(stderr, "Error: objfile is a required argument\n");
        print_usage_error();
        exit(1);
    }

    return objfile;
}

/*
    char first_inst[4];
    read(fd, first_inst, 4);
    printf("first inst: %x %x %x %x\n", first_inst[0], first_inst[1], first_inst[2], first_inst[3]);
*/

static void setup_and_run(char *filename) {
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

    run_machine((unsigned char *)ops, s.st_size);

    munmap(ops, s.st_size);
    close(fd);
}

int main(int argc, char **argv) {
    char *objfile = handle_args(argc, argv);

    printf("z80em - a z80 emulator and debugger\n");
    printf("Written by Sam Kingston <sam@sjkwi.com.au>\n");
    if (verbose) printf("  -v has enabled verbose execution\n");
    printf("\n");

    setup_and_run(objfile);

    return 0;
}
