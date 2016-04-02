#include <sys/types.h>
#include <sys/wait.h>
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
    printf("Starting %s\n", executable);

    // Allow other programs to trace this process.
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace");
        return -1;
    }

    execl(executable, executable, NULL);
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
    int process_status;
    unsigned intr_count = 0;

    // Wait for the process to change state, so that we know when it goes to
    // the next (in this case, first) instruction.
    wait(&process_status);
    do {
        struct user_regs_struct registers = get_registers(pid);
        unsigned curr_instr = ptrace(PTRACE_PEEKTEXT, pid, registers.rip, 0);
        log_info("icount = %u, EIP = 0x%08x. instr = 0x%08x", intr_count, registers.rip, curr_instr);

        if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0) {
            perror("ptrace");
            return;
        }

        wait(&process_status);
        intr_count++;
    } while (WIFSTOPPED(process_status));

    return;
}
