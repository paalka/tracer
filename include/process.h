#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/user.h>

#include "breakpoint.h"

struct program_context {
  char      *target_program;
  uintptr_t *bp_addrs;
  breakpoint_t **breakpoints;
  size_t     num_breakpoints;
  pid_t      pid;
} program_context;

struct user_regs_struct get_registers(pid_t pid);
int continue_execution(pid_t target_pid);
void start_target(const char *executable);
void resume_after_stop(struct program_context *context);
void stop_target(pid_t pid);
void set_registers(struct user_regs_struct new_registers, pid_t pid);
int do_single_step(pid_t pid);
void print_registers(pid_t pid);

struct program_context *init_program_context(char *program_name);
void destroy_context(struct program_context *context);

#endif
