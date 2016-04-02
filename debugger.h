#ifndef _DEBUGGER_H
#define _DEBUGGER_H
#include <sys/types.h>

void init_debugger(char *executable);
void start_target(char *executable);
void attach_debugger(pid_t pid);

#endif
