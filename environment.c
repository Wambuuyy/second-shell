#include "shell.h"

/**
 * set_environment_variable - Set or initialize an environment variable.
 * @variable: Name of the environment variable.
 * @value: Value to set for the environment variable.
 */
void set_environment_variable(const char *variable, const char *value)
{
	if (strcmp(variable, "PATH") == 0)
	{
		char *current_path = getenv("PATH");
		char *new_path = malloc(strlen(current_path) + strlen(value) + 2);

		if (new_path == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		sprintf(new_path, "%s:%s", current_path, value);
		if (setenv(variable, new_path, 1) != 0)
		{
			perror("setenv");
			fprintf(stderr, "Error: Unable to set environment variable %s\n", variable);
		}

		free(new_path);
	}
	if (setenv(variable, value, 1) != 0)
	{
		perror("setenv");
		fprintf(stderr, "Error: Unable to set environment variable %s\n", variable);
	}
}

/**
 * unset_environment_variable - Unset or remove an environment variable.
 * @variable: Name of the environment variable to unset.
 */
void unset_environment_variable(const char *variable)
{
	if (unsetenv(variable) != 0)
	{
		perror("unsetenv");
		fprintf(stderr, "Error: Unable to unset environment variable %s\n", variable);
	}
}

