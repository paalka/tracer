#include "../include/dbg.h"
#include "../include/prompt.h"
#include "../include/process.h"
#include "../include/debugger.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char **tokenize_str(char line[])
{
  /*
   * Divides the string 'string' into several strings.
   * The string is divided on each position where the char 'delimiter' is
   * found.
   * Returns a array of strings.
   */

  int bufsize = TOK_BUFSIZE;
  int position = 0;
  char **tokens = malloc(sizeof(char*) * bufsize);
  check(tokens != NULL, "Memory allocation failed.");

  char *token = strtok(line, TOK_DELIMITERS);

  while (token) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, sizeof(char*) * bufsize);
      check(tokens != NULL, "Memory allocation failed.");
              
    }



    // Pass NULL to obtain the next token.
    token = strtok(NULL, TOK_DELIMITERS);
        
  }

  // Descrease the size of the array to avoid using unnecessary space.
  tokens = realloc(tokens, sizeof(char*) * (position+1));
  check(tokens != NULL, "Memory reallocation failed.");
  tokens[position] = NULL;

  return tokens;

 error:
  if(tokens) free(tokens);
  exit(-1);
  
}

char *readline(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  check(buffer != NULL, "Allocation error.");

  while (1) {
    c = getchar();

    // If we hit EOF or a new line, replace it with a null character and
    // return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
          
    } else {
      buffer[position] = c;
          
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      check(buffer != NULL, "Allocation error.");
          
    }
      
  }

  return buffer;



 error:
  exit(EXIT_FAILURE);
  
}

char *get_user_input(void)
{
  /*
   * Obtains one or multiple lines of input from the user through stdin, and
   * adds it to the history.
   * Returns the string given by the user.
   */

  char *input = readline();
  return input;
}

void print_prompt(void)
{
  printf("%s%s\n> ", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
  return;
}


int handle_input(struct program_context *program_context, char **tokenized_input)
{
  char *cmd = tokenized_input[0];

  if (strcmp(cmd, "run") == 0) {
    init_debugger(program_context);
  } else if (strcmp(cmd, "b") == 0) {
    uintptr_t new_bp_addr = (uintptr_t) strtol(tokenized_input[1], NULL, 16);
    add_breakpoint(program_context, new_bp_addr);
  } else if (strcmp(cmd, "cont") == 0) {
    if (program_context->pid > 0) {
      resume_after_stop(program_context);
    } else {
      printf("Program not running!");
    }
  } else if (strcmp(cmd, "r") == 0) {
    print_registers(program_context->pid);
  }

  return 0;
}

void run_main_loop(char *target_program)
{
  char *input;
  char **tokenized_input;
  int process_status;

  struct program_context *current_context = init_program_context(target_program);

  do {
    print_prompt();
    input = get_user_input();
    if (strcmp(input, "") != 0) {
      tokenized_input = tokenize_str(input);
      process_status = handle_input(current_context, tokenized_input);

      free(tokenized_input);
    }
  } while (1);

  destroy_context(current_context);
}
