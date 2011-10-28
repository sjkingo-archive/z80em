#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dbg.h"
#include "emulator.h"

static bool enable_dbg = false;

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

int main(int argc, char **argv) {
    char *objfile = handle_args(argc, argv);
    void (*debugger_callback)(void) = dbg_break;

    printf("z80em - a z80 emulator and debugger\n");
    printf("Written by Sam Kingston <sam@sjkwi.com.au>\n");
    if (verbose) printf("-v has enabled verbose execution\n");
    printf("\n");

    /* set up and run the emulation */
    struct emulator_state *s = init_emulator(objfile, enable_dbg,
            NULL, debugger_callback);
    run_machine(s);

    printf("end of emulation\n");
    deinit_emulator(s);

    return 0;
}
