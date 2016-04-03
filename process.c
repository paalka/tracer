#include <sys/ptrace.h>
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
    ptrace(PTRACE_SETREGS, pid, 0, new_registers);
}


int continue_execution(pid_t target_pid)
{
    int return_code = ptrace(PTRACE_CONT, target_pid, 0, 0);
    if (return_code < 0) {
        perror("ptrace");
        return ERROR;
    } else {
        return SUCCESS;
    }
}
