#include "shell.h"

void execute_command(const char *const *command)
{
	char *cmd_path;
	pid_t pid;
	
	if (strcmp(command[0], "cd") == 0)
	{
		/* Handle 'cd' command*/
		if (command[1] != NULL)
		{
			if (chdir(command[1]) == -1)
			{
				perror("chdir");
			}
		}
		else
		{
			fprintf(stderr, "Error: 'cd' command requires a directory argument.\n");
		}
		return;
	}
	else if (strcmp(command[0], "env") == 0)
	{
		print_environment();		
		return;
	}

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
		continue;/*continue the shell loop*/
	}
	else if (pid == 0)
	{
		if (execve(cmd_path, (char *const *)command , environ) == -1)
		{
			/* If execve fails*/
			perror("execve");
			exit(EXIT_FAILURE);
		}
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
		/*	else
			{
				printf("Command executed successfully.\n");
			}*/
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
	if (command[0] == '/')/*|| command[0] == '.')*/
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
			if (cmd_path == NULL)
			{
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			snprintf(cmd_path, strlen(token) + strlen(command) + 2, "%s/%s", token, command);
			/*Check if the command exists at the constructed path*/
			if (access(cmd_path, X_OK) == 0)
			{
				break;
			}
			/* Free the memory for the current path and prepare for the next iteration */
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
	/*printf("Resolved command path: %s\n", cmd_path);  Debugging statement */
	return cmd_path;
}

void print_environment()
{
	char **env_var = environ;

	while (*env_var != NULL)
	{
		printf("%s\n", *env_var);
		env_var++;
	}
}
