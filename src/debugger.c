#include <stdlib.h>

#include "../include/util.h"
#include "../include/prompt.h"

void init_debugger(struct program_context *context)
{
  pid_t child_pid = fork();

  if (child_pid == 0) {
    start_target(context->target_program);
  } else if (child_pid > 0) {
    context->pid = child_pid;

    stop_target(child_pid);
    context->breakpoints = malloc(sizeof(breakpoint_t) * context->num_breakpoints);
    for (size_t i = 0; i < context->num_breakpoints; i++) {
      breakpoint_t *new_bp = create_breakpoint(context->bp_addrs[i], child_pid);
      context->breakpoints[i] = new_bp;
    }
    continue_execution(child_pid);
  }
}

void add_breakpoint(struct program_context *program_context, uintptr_t new_bp_addr)
{
  size_t curr_num_bps = program_context->num_breakpoints;
  for (size_t i = 0; i < curr_num_bps; i++) {
    if (program_context->bp_addrs[i] == 0) {
	program_context->bp_addrs[i] = new_bp_addr;
	return;
      }
  }

  program_context->bp_addrs = realloc(program_context->bp_addrs, curr_num_bps*2);
  program_context->bp_addrs[curr_num_bps+1] = new_bp_addr;
  program_context->num_breakpoints = curr_num_bps*2;
}
 
int main(int argc, char** argv)
{
  if (argc < 2) {
    log_error("Name of program to debug was not provided!");
    return -1;
  }

  run_main_loop(argv[1]);
}
