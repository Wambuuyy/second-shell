#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

extern char **environ;  /* Declare the external variable to access the environment */

#define BUFFER_SIZE 1024

/*static char buffer[BUFFER_SIZE];
static size_t buffer_index = 0;*/
char *str_tok(char *str, const char *delimiters);
void execute_command(const char *const *command);
char *custom_getline();
void set_environment_variable(const char *variable, const char *value);
void unset_environment_variable(const char *variable);
void tokenize(char *input, char *command[], int max_args);
char *resolve_command_path(const char *command);
void print_environment();

#endif

