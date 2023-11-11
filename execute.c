#include "shell.h"

void execute_command(const char *const *command)
{
	char *cmd_path;
	pid_t pid;

	cmd_path = resolve_command_path(command[0]);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "Error: Command not found in PATH.\n");
		return;
	}		
	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execve(cmd_path, (char *const *)command , environ);

		/* If execve fails*/
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process*/
		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			{
				fprintf(stderr, "Error: The command exited with a non-zero status.\n");
			}
			else
			{
				printf("Command executed successfully.\n");
			}
		}
		else
		{
			fprintf(stderr, "Error: The command exited with a non-zero status.\n");
		}
	}
	free(cmd_path);
}

char *resolve_command_path(const char *command)
{
	char *cmd_path = NULL;

	/* Check if the command is an absolute or relative path*/
	if (command[0] == '/' || command[0] == '.')
	{
		cmd_path = strdup(command);
	}
	else
	{
		/* Search for the command in the PATH environment variable*/
		char *path = getenv("PATH");
		char *token = strtok(path, ":");
		while (token != NULL)
		{
			/* Construct the full path of the command*/
			cmd_path = malloc(strlen(token) + strlen(command) + 2);
			sprintf(cmd_path, "%s/%s", token, command);
			/*Check if the command exists at the constructed path*/
			if (access(cmd_path, X_OK) == 0)
			{
				break;
			}
			free(cmd_path);
			cmd_path = NULL;
			token = strtok(NULL, ":");
		}
		if (cmd_path == NULL)
		{
			fprintf(stderr, "Error: Command not found in any PATH directory.\n");
			return (NULL);
		}
	}
	printf("Resolved command path: %s\n", cmd_path);  /* Debugging statement */
	return cmd_path;
}
