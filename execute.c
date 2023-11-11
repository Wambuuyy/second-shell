#include "shell.h"

void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/*Child process*/
		char *args[2];
		args[0] = command;
		args[1] = NULL;
		execve(command, args, environ);

		/* If execve fails*/
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process*/
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			fprintf(stderr, "Error: The command exited with a non-zero status.\n");
		}
	}
}
