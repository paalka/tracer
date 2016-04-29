#include <stdlib.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

#include "breakpoint.h"
#include "util.h"
#include "process.h"

breakpoint_t *create_breakpoint(uintptr_t addr, pid_t pid)
{
    breakpoint_t *new_bp = malloc(sizeof(breakpoint_t));
    new_bp->addr = addr;
    new_bp->pid = pid;
    new_bp->instr = ptrace(PTRACE_PEEKTEXT, pid, addr, NO_DATA);

    // Make the instruction at this address into a 'int 3' i.e. breakpoint
    // instruction.
    new_bp->trap_instr = (LAST_TWO_BITS_OF(new_bp->instr) | TRAP_INST);
    ptrace(PTRACE_POKETEXT, pid, addr, new_bp->trap_instr);

    return new_bp;
}

void remove_breakpoint(breakpoint_t *bp_to_remove)
{
    ptrace(PTRACE_POKETEXT,
           bp_to_remove->pid,
           bp_to_remove->addr,
           bp_to_remove->instr);

}

void delete_breakpoint(breakpoint_t *breakpoint)
{
    free(breakpoint);
}

int resume_after_breakpoint(breakpoint_t* bp)
{
    struct user_regs_struct regs = get_registers(bp->pid);
    int process_status;

    remove_breakpoint(bp);

    // Rewind the instruction pointer.
    regs.NEXT_INST_PTR--;
    set_registers(regs, bp->pid);

    process_status = continue_execution(bp->pid);

    if (WIFEXITED(process_status)) {
        log_info("Child exited\n");
    }
    else {
        log_info("Unexpected signal\n");
        return ERROR;
    }

    return SUCCESS;
}
