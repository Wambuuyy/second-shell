#include "shell.h"

int main()
{
	char input[MAX_COMMAND_LENGTH];
	char *command[20];

	while (1)
	{
		printf("Pru$ ");
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			if (feof(stdin))
			{
				printf("\n");/*print new line after EOF*/
				break;  /* Exit the loop on Ctrl+D*/
			}
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		/* Remove the newline character*/
		input[strcspn(input, "\n")] = '\0';

		/* exit if the user enters "exit" or "quit"*/
		if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0)
			break;
		tokenize(input, command, 20);
		/* Execute the command only if its not empty*/
		execute_command((const char *const *)command);
	}
	return 0;
}

void tokenize(char *input, char *command[], int max_args)
{
	char *token = strtok(input, " ");
	int i = 0;

	while (token != NULL && i < max_args)
	{
		command[i++] = token;
		token = strtok(NULL, " ");
	}
	command[i] = NULL;
}
