#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "cpu.h"
#include "dbg.h"
#include "emulator.h"

bool verbose = false;

static inline void print_usage_error(void) {
    fprintf(stderr, "Usage: z80em [options..] objfile\n");
    fprintf(stderr, "\t-d\tenter the dbg upon start\n");
    fprintf(stderr, "\t-h\tprint this usage message and exit\n");
    fprintf(stderr, "\t-v\tenable verbose execution\n");
}

static char *handle_args(int argc, char **argv) {
    int opt;

    while ((opt = getopt(argc, argv, "dhv")) != -1) {
        switch (opt) {
            case 'd':
                enable_dbg = true;
                break;
            case 'h':
                print_usage_error();
                exit(0);
            case 'v':
                verbose = true;
                break;
            default:
                print_usage_error();
                exit(1);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Error: objfile is a required argument\n");
        print_usage_error();
        exit(1);
    }

    return argv[optind];
}

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
    if (verbose) printf("-v has enabled verbose execution\n");
    printf("\n");

    init_cpu_state();
    if (enable_dbg)
        dbg_init(NULL);
    setup_and_run(objfile);

    printf("end of emulation\n");
    return 0;
}
