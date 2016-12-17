#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "../include/process.h"
#include "../include/util.h"

#define NO_MORE_ARGS NULL

struct user_regs_struct get_registers(pid_t pid)
{
  struct user_regs_struct registers;
  ptrace(PTRACE_GETREGS, pid, NO_ADDR, &registers);
  return registers;
}

void print_registers(pid_t pid)
{
  struct user_regs_struct curr_regs;
  ptrace(PTRACE_GETREGS, pid, NO_ADDR, &curr_regs);

  printf("Registers for pid: %d\n", pid);
  printf("RIP: %llx\tRSP: %llx\t \n", curr_regs.rip, curr_regs.rsp);
  printf("RAX: %llx\tRBP: %llx\t \n", curr_regs.rax, curr_regs.rbp);
  printf("RBX: %llx\tRCX: %llx\t \n", curr_regs.rbx, curr_regs.rcx);
  printf("RDI: %llx\tRDX: %llx\t \n", curr_regs.rdi, curr_regs.rdx);
  printf("RSI: %llx\tR10: %llx\t \n", curr_regs.rsi, curr_regs.r10);
  printf("R11: %llx\tR12: %llx\t \n", curr_regs.r11, curr_regs.r12);
  printf("R13: %llx\tR14: %llx\t \n", curr_regs.r13, curr_regs.r14);
  printf("R15: %llx\n", curr_regs.r15);
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

void stop_target(pid_t pid)
{
  int process_status = NULL;
  int wait_status =  wait(&process_status);

  if (wait_status < 0) {
    log_error("Failed waiting for process: %d", pid);
    return;
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

void resume_after_stop(struct program_context *context) {
  struct user_regs_struct curr_regs = get_registers(context->pid);

  for (size_t i = 0; i < context->num_breakpoints; i++) {
    breakpoint_t *curr_bp = context->breakpoints[i];
    if (curr_regs.rip-1 == curr_bp->addr) {
      resume_after_breakpoint(curr_bp);
      delete_breakpoint(curr_bp);
      break;
    }
  }
}  

int continue_execution(pid_t target_pid)
{
  int process_status = NULL;
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

struct program_context *init_program_context(char *program_name)
{
  struct program_context *new_context = malloc(sizeof(program_context));
  new_context->target_program = program_name;

  const size_t DEFAULT_NUM_BPS = 3;
  new_context->bp_addrs = malloc(sizeof(uintptr_t) * DEFAULT_NUM_BPS);
  new_context->num_breakpoints = DEFAULT_NUM_BPS;
  new_context->pid = -1;

  return new_context;
}

void destroy_context(struct program_context *context)
{
  if (context->bp_addrs != NULL) {
    free(context->bp_addrs);
  }
  if (context != NULL) {
    free(context);
  }
}
