#ifndef _BREAKPOINT_H
#define _BREAKPOINT_H

#include <sys/types.h>
#include <stdint.h>

typedef struct {
      uintptr_t addr;
      uintptr_t orig_data;
      uintptr_t trap_data;
      pid_t pid;
} breakpoint_t;

breakpoint_t* create_breakpoint(uintptr_t addr, pid_t pid);
void remove_breakpoint(breakpoint_t *bp_to_remove);

#endif
