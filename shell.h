#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;  /* Declare the external variable to access the environment */

#define MAX_COMMAND_LENGTH 100

void execute_command(char *command);
#endif

