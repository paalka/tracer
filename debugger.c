#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include <stdint.h>

#include "debugger.h"
#include "breakpoint.h"
#include "util.h"
#include "process.h"

void init_debugger(char *executable)
{
    pid_t child_pid = fork();

    if (child_pid == 0) {
        start_target(executable);
    }
    else if (child_pid > 0) {
        attach_debugger(child_pid);
    }
}

void attach_debugger(pid_t child_pid)
{
    int wait_status;
    wait(&wait_status);

    uintptr_t addr = 0x4000c6;
    breakpoint_t *new_bp = create_breakpoint(addr, child_pid);

    ptrace(PTRACE_CONT, child_pid, 0, 0);
    wait(&wait_status);

    if (WIFSTOPPED(wait_status)) {
        log_info("Child got a signal: %d\n", WSTOPSIG(wait_status));
    }
    else {
        perror("wait");
        return;
    }

    resume_after_breakpoint(new_bp);
    delete_breakpoint(new_bp);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        log_error("Name of program to debug was not provided!");
        return -1;
    }

    init_debugger(argv[1]);
}
