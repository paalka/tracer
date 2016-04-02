#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/types.h>
#include <sys/user.h>

struct user_regs_struct get_registers(pid_t pid);
int continue_execution(pid_t target_pid);

#endif
