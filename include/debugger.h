#ifndef _DEBUGGER_H
#define _DEBUGGER_H
#include <stdint.h>
#include "../include/prompt.h"

void init_debugger(struct program_context *context);
void attach_debugger(pid_t pid);
void add_breakpoint(struct program_context *program_context, uintptr_t new_bp_addr);

#endif
