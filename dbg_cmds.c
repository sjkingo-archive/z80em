#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "cpu.h"
#include "dbg.h"
#include "emulator.h"

static void cmd_cont(char **args __attribute__((unused))) {
    if (dbg_cont_possible)
        enable_dbg = false;
    else
        printf("dbg: cannot continue due to previous error\n");
}

static void cmd_step(char **args __attribute__((unused))) {
    if (dbg_cont_possible) {
        enable_dbg = false;
        dbg_ss = true;
    } else {
        printf("dbg: cannot step due to previous error\n");
    }
}

static void do_cmd_show(char *cmd, char **args, FILE *out) {
    if (args[0] == NULL) {
        fprintf(out, "%s [option]\n", cmd);
        fprintf(out, "     regs\tShow the status of the CPU registers\n");
        fprintf(out, "     objfile\thexdumps the object file from pc point on\n");
        fprintf(out, "see also less\n");
        return;
    }

    if (strcmp(args[0], "regs") == 0) {
        print_regs(out);
    } else if (strcmp(args[0], "objfile") == 0) {
        dump_objfile(out);
    }
}

static void cmd_show(char **args) {
    /* wrapper due to file stream mangling */
    do_cmd_show("show", args, stdout);
}

static void cmd_less(char **args) {
    /* honour the user's pager setting */
    char *pager = getenv("PAGER");
    if (pager == NULL) {
        printf("Cannot use this command as PAGER is not set in env\n");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }

    /* ignore broken pipes */
    signal(SIGPIPE, SIG_IGN);

    int pid = fork();
    if (pid == 0) {
        /* redirect the pipe to stdin and exec less */
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("child dup2");
            _exit(10);
        }
        execlp(pager, pager, NULL);
        _exit(0);
    } else {
        /* send the output of do_cmd_show() down the pipe */
        close(pipefd[0]);
        int status;
        FILE *out = fdopen(pipefd[1], "w");
        if (out == NULL) {
            perror("fdopen");
            return;
        }
        do_cmd_show("less", args, out);
        fflush(out);
        fclose(out);
        close(pipefd[1]);
        wait(&status);
    }
}

static void cmd_set(char **args) {
    if (args[0] == NULL || args[1] == NULL) {
        printf("set A|B|C|D|E|H|L|pc n\n");
        return;
    }

    long n = strtol(args[1], NULL, 0);
    
    if (strcmp(args[0], "pc") == 0) {
        set_pc(n);
    } else {
        switch (args[0][0]) {
            case 'A':
                set_reg(REG_A, n);
                break;
            case 'B':
                set_reg(REG_B, n);
                break;
            case 'C':
                set_reg(REG_C, n);
                break;
            case 'D':
                set_reg(REG_D, n);
                break;
            case 'E':
                set_reg(REG_E, n);
                break;
            case 'H':
                set_reg(REG_H, n);
                break;
            case 'L':
                set_reg(REG_L, n);
                break;
            default:
                printf("unknown register %c\n", args[0][0]);
                return;
        }
    }
}

static void cmd_help(char **args __attribute__((unused))) {
    unsigned int i = 0;
    while (dbg_cmds[i].cmd != NULL) {
        printf("%s\t\t%s\n", dbg_cmds[i].cmd, dbg_cmds[i].help);
        i++;
    }
}

struct dbg_cmd_entry dbg_cmds[] = {
    { "c", &cmd_cont, "Continue execution." },
    { "s", &cmd_step, "Single-step execution." },
    { "show", &cmd_show, "Show various information about the emulation." },
    { "less", &cmd_less, "Show various information about the emulation, piped to less." },
    { "set", &cmd_set, "Set various registers." },

    { "help", &cmd_help, "Show this help information." },
    { NULL, NULL, NULL }, /* sentinel entry; don't remove */
};

