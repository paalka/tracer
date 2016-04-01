#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "debugger.h"
#include "util.h"

void init_debugger(char *executable)
{
    pid_t child_pid = fork();

    if (child_pid == 0) {
        int status = start_target(executable);
    }
    else if (child_pid > 0) {
        attach_debugger(child_pid);
    }
}

int start_target(char *executable)
{
    return 0;
}

void attach_debugger(pid_t pid)
{
}
