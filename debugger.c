#include <sys/types.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/ptrace.h>
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
    return SUCCESS;
}

struct user_regs_struct get_registers(pid_t pid)
{
    struct user_regs_struct registers;
    ptrace(PTRACE_GETREGS, pid, 0, &registers);
    return registers;
}

void attach_debugger(pid_t pid)
{
}
