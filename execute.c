#include "shell.h"

void execute_command(const char *const *command)
{
	char *cmd_path;
	pid_t pid;
	const char *target_dir;
	char cwd[PATH_MAX];
	
	if (strcmp(command[0], "cd") == 0)
	{
		/* Handle 'cd' command*/
		target_dir = (command[1] != NULL) ? command[1] : getenv("HOME");

		if (chdir(target_dir) == -1)
		{
			perror("chdir");
		}
		else
		{
			/* Update PWD environment variable */
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				set_environment_variable("PWD", cwd);
			}
			else
			{
				perror("getcwd");
			}
		}
		return;
	}
	else if (strcmp(command[0], "env") == 0)
	{
		print_environment();		
		return;
	}

	else if (strcmp(command[0], "setenv") == 0)
	{
		/* Handle 'setenv' command*/
		if (command[1] != NULL && command[2] != NULL)
		{
			set_environment_variable(command[1], command[2]);
		}
		else
		{
			fprintf(stderr, "Error: 'setenv' command requires both VARIABLE and VALUE arguments.\n");
		}
		return;
	}
	else if (strcmp(command[0], "unsetenv") == 0)
	{
		/*Handle 'unsetenv' command*/
		if (command[1] != NULL) {
		unset_environment_variable(command[1]);
	}
		else
		{
			fprintf(stderr, "Error: 'unsetenv' command requires a VARIABLE argument.\n");
		}
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
		exit(EXIT_FAILURE);/*continue the shell loop*/
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
	char *path = getenv("PATH");
	char *token = str_tok(path, ":");

	/* Check if the command is an absolute or relative path*/
	if (command[0] == '/')/*|| command[0] == '.')*/
	{
		cmd_path = strdup(command);
	}
	else
	{
		printf("PATH: %s\n", path);

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

			token = str_tok(NULL, ":");
		}
		if (cmd_path == NULL)
		{
			fprintf(stderr, "Error: Command not found in any PATH directory.\n");
			return (NULL);
		}
	}
	printf("Resolved command path: %s\n", cmd_path);/* Debugging statement */
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
