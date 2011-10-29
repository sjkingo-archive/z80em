#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "dbg.h"
#include "emulator.h"

#define TMP_FILE "./_test.bin"

static void print_usage_error(void) {
    fprintf(stderr, "Usage: test -a <path-to-asm>       run tests with the given assembler\n");
    fprintf(stderr, "            -h                     display this help message\n");
}

static char *handle_args(int argc, char **argv) {
    char *assembler = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "a:h")) != -1) {
        switch (opt) {
            case 'a':
                assembler = optarg;
                break;
            case 'h':
                print_usage_error();
                exit(0);
            default:
                print_usage_error();
                exit(1);
        }
    }

    if (assembler == NULL) {
        fprintf(stderr, "Error: -a is a required argument\n");
        print_usage_error();
        exit(1);
    }

    return assembler;
}

pid_t setup_assembler(char *asm_bin, char *fifo_in, char *fifo_out) {
    pid_t pid = fork();
    if (pid == 0) {
        execl(asm_bin, asm_bin, "-l", "-L", "-o", fifo_out, fifo_in, NULL);
        perror("execl");
        _exit(10);
    } else {
        return pid;
    }
}

void cleanup_assembler(pid_t pid) {
    /* finished, clean up the assembler */
    int status;
    printf("going to wait for assembler\n");
    fflush(stdout);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Warning: assembler exited with status %d\n", 
                    WEXITSTATUS(status));
        } else {
            printf("assembler exited cleanly\n");
        }
    } else {
        printf("assembler hasn't exited?\n");
    }
    fflush(stdout);
}

void run_test(char *asm_bin, char *fifo_in, char *fifo_out) {
    pid_t asm_pid;
    struct emulator_state *s;

    asm_pid = setup_assembler(asm_bin, fifo_in, fifo_out);
    printf("assembler set up\n");
    fflush(stdout);

    /* set up the emulator in a child so it doesn't block the parent*/
    pid_t em_pid = fork();
    if (em_pid == 0) {
        s = init_emulator(fifo_out, false, NULL, NULL);
        verbose = true;
        dbg_enable_ss();
        printf("emulator set up\n");
        fflush(stdout);
        run_machine(s);
        deinit_emulator(s);
        _exit(0);
    }

    /* assemble op and run it */
    FILE *f = fopen(fifo_in, "w");
    if (f == NULL) {
        perror("fopen (in)");
        exit(5);
    }
    fprintf(f, "ld a,0\n");
    fclose(f);
    printf("wrote to assembler\n");
    fflush(stdout);

    cleanup_assembler(asm_pid);
    int status;
    waitpid(em_pid, &status, 0);
}

int main(int argc, char **argv) {
    char *assembler = handle_args(argc, argv);

    printf("z80em test program\n");
    printf("using %s as assembler\n", assembler);

    /* run assembler, in -> /dev/stdin and out -> TMP_FILE */

    /* run emulator, in -> TMP_FILE */

    unlink(TMP_FILE);

    return 0;
}
