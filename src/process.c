#include <sys/ptrace.h>
#include <sys/wait.h>

#include "../include/process.h"
#include "../include/util.h"

#define NO_MORE_ARGS NULL

struct user_regs_struct get_registers(pid_t pid)
{
    struct user_regs_struct registers;
    ptrace(PTRACE_GETREGS, pid, NO_ADDR, &registers);
    return registers;
}

void set_registers(struct user_regs_struct new_registers, pid_t pid)
{
    ptrace(PTRACE_SETREGS, pid, NO_ADDR, &new_registers);
}

int do_single_step(pid_t pid)
{
    if (ptrace(PTRACE_SINGLESTEP, pid, NO_ADDR, NO_DATA)) {
            perror("ptrace");
            return ERROR;
    } else {
        return SUCCESS;
    }
}

void start_target(const char *executable)
{
    log_info("Starting %s", executable);

    // Allow other programs to trace this process.
    if (ptrace(PTRACE_TRACEME, THIS_PID, NO_ADDR, NO_DATA) < 0) {
        log_error("Could not enable ptrace for this process.");
        perror("ptrace");
        return;
    }

    execl(executable, executable, NO_MORE_ARGS);
}

int continue_execution(pid_t target_pid)
{
    int process_status;
    int return_code = ptrace(PTRACE_CONT, target_pid, NO_ADDR, NO_DATA);
    wait(&process_status);

    if (return_code < 0) {
        log_error("Could not continue execution.");
        perror("ptrace");
        return process_status;
    } else {
        return process_status;
    }
}
