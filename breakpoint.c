#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>

#include "breakpoint.h"
#include "util.h"
#include "process.h"

breakpoint_t *create_breakpoint(unsigned addr, pid_t pid)
{
    breakpoint_t *new_bp = malloc(sizeof(breakpoint_t));
    new_bp->addr = addr;
    new_bp->pid = pid;
    new_bp->orig_data = ptrace(PTRACE_PEEKTEXT, pid, addr, 0);

    // Add the 'int 3' instruction to the address.
    new_bp->trap_data = (LAST_TWO_BITS_OF(new_bp->orig_data) | TRAP_INST);

    ptrace(PTRACE_POKETEXT, pid, addr, new_bp->trap_data);

    return new_bp;
}

void remove_breakpoint(breakpoint_t *bp_to_remove)
{
    ptrace(PTRACE_POKETEXT, bp_to_remove->pid, bp_to_remove->addr,
           (LAST_TWO_BITS_OF(bp_to_remove->trap_data)
            | (bp_to_remove->orig_data & 0xFF)));

}

void delete_breakpoint(breakpoint_t *breakpoint)
{
    free(breakpoint);
}

void resume_after_breakpoint(breakpoint_t *breakpoint)
{
    struct user_regs_struct registers;

}
