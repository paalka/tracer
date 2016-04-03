#include <sys/ptrace.h>
#include <sys/wait.h>
#include "process.h"
#include "util.h"

struct user_regs_struct get_registers(pid_t pid)
{
    struct user_regs_struct registers;
    ptrace(PTRACE_GETREGS, pid, 0, &registers);
    return registers;
}

void set_registers(struct user_regs_struct new_registers, pid_t pid)
{
    ptrace(PTRACE_SETREGS, pid, 0, &new_registers);
}

int do_single_step(pid_t pid)
{
    if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0)) {
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
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        log_error("Could not enable ptrace for this process.");
        perror("ptrace");
        return;
    }

    execl(executable, executable, NULL);
}

int continue_execution(pid_t target_pid)
{
    int process_status;
    int return_code = ptrace(PTRACE_CONT, target_pid, 0, 0);
    wait(&process_status);

    if (return_code < 0) {
        log_error("Could not continue execution.");
        perror("ptrace");
        return process_status;
    } else {
        return process_status;
    }
}
