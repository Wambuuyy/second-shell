#include "shell.h"

char **strdup_command_array(const char *const *array)
{
	int size = 0, i;
	char **new_array;

	while (array[size] != NULL)
	{
		size++;
	}

	new_array = malloc((size + 1) * sizeof(char *));
	if (new_array == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < size; i++)
	{
		new_array[i] = strdup(array[i]);
		if (new_array[i] == NULL)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
	}

	new_array[size] = NULL;
	return new_array;
}

void free_command_array(char **array)
{
	int i;

	for (i = 0; array[i] != NULL; i++)
	{
		free(array[i]);
	}
	free(array);
}
