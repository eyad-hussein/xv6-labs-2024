#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
run_command(char *cmd, char **cmd_args, char *line)
{
    int pid;
    char *args[MAXARG];
    int i = 0;

    for (; cmd_args[i] != 0; i++) {
        args[i] = cmd_args[i];
    }

    args[i++] = line;
    args[i] = 0;

    if ((pid = fork()) == 0) {
        exec(cmd, args);
        fprintf(2, "xargs: exec failed\n");
        exit(1);
    } else if (pid > 0) {
        wait(0);
    } else {
        fprintf(2, "xargs: fork failed\n");
        exit(1);
    }
}

int
main(int argc, char *argv[])
{
    char buf[512];
    char *cmd;
    char *cmd_args[MAXARG];
    int n, i;

    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    cmd = argv[1];
    for (i = 1; i < argc && i < MAXARG; i++) {
        cmd_args[i - 1] = argv[i];
    }
    cmd_args[i - 1] = 0;

    while ((n = read(0, buf, sizeof(buf))) > 0) {
        int start = 0;

        for (int j = 0; j < n; j++) {
            if (buf[j] == '\n') {
                buf[j] = 0;
                run_command(cmd, cmd_args, &buf[start]);
                start = j + 1;
            }
        }
    }

    exit(0);
}

