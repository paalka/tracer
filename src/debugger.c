#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/debugger.h"
#include "../include/breakpoint.h"
#include "../include/util.h"
#include "../include/process.h"

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
    int process_status = NULL;
    int wait_status =  wait(&process_status);

    if (wait_status < 0) {
        log_error("Failed waiting for process: %d", child_pid);
        return;
    }

    uintptr_t addr = 0x4000c6;
    breakpoint_t *new_bp = create_breakpoint(addr, child_pid);

    process_status = continue_execution(new_bp->pid);

    if (WIFSTOPPED(process_status)) {
        log_info("Child got a signal: %d\n", WSTOPSIG(process_status));
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
