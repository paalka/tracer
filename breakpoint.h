#ifndef _BREAKPOINT_H
#define _BREAKPOINT_H

#include <sys/types.h>

typedef struct {
      unsigned addr;
      unsigned orig_data;
      unsigned trap_data;
      pid_t pid;
} breakpoint_t;

breakpoint_t* create_breakpoint(unsigned addr, pid_t pid);
void remove_breakpoint(breakpoint_t *bp_to_remove);

#endif
