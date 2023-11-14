#include "shell.h"

int main()
{
	char *input = NULL;
	size_t input_size = 0;
	char *command[20];
	int stat_code;
	int ex;

	while (1)
	{
		printf("Pru$ ");
		if (getline(&input, &input_size, stdin) == -1)
		{
			if (feof(stdin))
			{
				printf("\n");/*print new line after EOF*/
				break;/* Exit the loop on Ctrl+D*/
			}
			perror("getline");
			exit(EXIT_FAILURE);
		}
		/* Remove the newline character*/
		input[strcspn(input, "\n")] = '\0';
		/* Check if the user pressed Enter without typing a command */
		if (input[0] == '\0')
		{
			continue;/* Skip the rest of the loop and prompt again */
		}
		/* exit if the user enters "exit" or "quit"*/
		if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0)
		{
			break;
		}
		ex = strlen("exit");
		if (strncmp(input, "exit", ex) == 0)
		{
			/* extract the status code from the input*/
			stat_code = atoi(input + ex);
			/*handle invalid status codes*/
			if (stat_code == 0 && input[ex] != '0')
			{
				fprintf(stderr, "Error: Invalid status code.\n");
			}
			else
			{
				/*exit the shell with the provided status code*/
				free(input);
				exit(stat_code);
			}
		}
		/* Handle the "env" built-in command */
		if (strcmp(input, "env") == 0)
		{
			print_environment();
			continue;
		}
		tokenize(input, command, 20);

		if (command[0] != NULL)
		{
			/* Execute the command only if its not empty*/
			execute_command((const char *const *)command);
		}
	}
	free(input);
	return 0;
}
