#ifndef _prompt_h
#define _prompt_h

#include <stdio.h>
#include "../include/process.h"

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIMITERS " \t\r\n\a"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

char **tokenize_str(char line[]);
char *readline(void);

int execute_command(char **args);

int *process_input(void);
void print_prompt(void);

void run_main_loop(char *target_program);

int handle_input(struct program_context *program_context, char **tokenized_input);

#endif
