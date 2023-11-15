#include "shell.h"

/**
 * execute_cd - Execute 'cd' command.
 * @command: Command to execute.
 */
void execute_cd(const char *const *command)
{
	const char *target_dir = (command[1] != NULL) ? command[1] : getenv("HOME");

	if (chdir(target_dir) == -1)
	{
		perror("chdir");
	}
	else
	{
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			set_environment_variable("PWD", cwd);
		}
		else
		{
			perror("getcwd");
		}
	}
}

/**
 * execute_env - Execute 'env' command.
 */
void execute_env()
{
	print_environment();
}
/**
 * execute_setenv - Execute 'setenv' command.
 * @command: Command to execute.
 */
void execute_setenv(const char *const *command)
{
	if (command[1] != NULL && command[2] != NULL)
	{
		set_environment_variable(command[1], command[2]);
	}
	else
	{
		fprintf(stderr, "Error: 'setenv' command requires both VARIABLE and VALUE arguments.\n");
	}
}

/**
 * execute_unsetenv - Execute 'unsetenv' command.
 * @command: Command to execute.
 */
void execute_unsetenv(const char *const *command)
{
	if (command[1] != NULL)
	{
		unset_environment_variable(command[1]);
	}
	else
	{
		fprintf(stderr, "Error: 'unsetenv' command requires a VARIABLE argument.\n");
	}
}

/**
 * execute_external_command - Execute an external command.
 * @command: Command to execute.
 * @command_length: Length of the command.
 */
void execute_external_command(const char *const *command, size_t command_length)
{
	char *last_arg = strdup(command[command_length - 1]);
	last_arg[strcspn(last_arg, "\n")] = '\0';

	char **mutable_command = strdup_command_array(command);
	char *cmd_path = resolve_command_path(mutable_command);

	if (cmd_path == NULL)
	{
		fprintf(stderr, "Error: Command not found in PATH.\n");
		free(last_arg);
		return;
	}

	printf("Resolved command path: %s\n", cmd_path);
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (command[0][0] == '/' || command[0][0] == '.')
		{
			if (execve(command[0], (char *const *)command, environ) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (execve(cmd_path, mutable_command, environ) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			{
				fprintf(stderr, "Error: The command exited with a non-zero status.\n");
			}
		}
		else
		{
			fprintf(stderr, "Error: The command exited with a non-zero status.\n");
		}
	}

	free(cmd_path);
	free_command_array(mutable_command);
}

/**
 * resolve_command_path - Resolve the full path of a command.
 * @command: Array containing the command and its arguments.
 *
 * This function constructs and returns the full path of the specified command
 * by searching through the directories listed in the PATH environment variable.
 * If the command is already an absolute or relative path, it is used as is.
 * The function prints debugging information during the search process.
 *
 * Return: A dynamically allocated string representing the full path of the command,
 * or NULL if the command is not found in any PATH directory.
 */
char *resolve_command_path(char **command)
{
	char *cmd_path = NULL;
	char *path = getenv("PATH");
	char *token = str_tok(path, ":");
	printf("PATH: %s\n", path);

	/* Check if the command is an absolute or relative path*/
	if (*command[0] == '/' || *command[0] == '.')
	{
		cmd_path = strdup(*command);
		set_environment_variable("PWD", *command);
	}
	else
	{
		while (token != NULL)
		{
			/* Construct the full path of the command*/
			cmd_path = malloc(strlen(token) + strlen(*command) + 2);
			if (cmd_path == NULL)
			{
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			snprintf(cmd_path, strlen(token) + strlen(*command) + 2, "%s/%s", token, *command);

			/* Print debugging information */
			printf("Trying path: %s\n", cmd_path);
			
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

/**
 * print_environment - Print the environment variables.
 *
 * This function prints the current environment variables line by line.
 * It iterates through the environment variable array and uses printf
 * to display each variable followed by a newline character.
 */
void print_environment()
{
	char **env_var = environ;

	while (*env_var != NULL)
	{
		printf("%s\n", *env_var);
		env_var++;
	}
}
