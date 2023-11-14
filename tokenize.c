#include "shell.h"
/**
 * str_tok - Function that tokenizes a string
 * @str: String to be tokenized
 * @delimiters: Characters used as delimiters
 *
 * Return: Pointer to the next token, NULL if no more tokens
 */
char *str_tok(char *str, const char *delimiters)
{
	static char *lastToken = NULL; /* To keep track of the last token */
	char *tokenStart = NULL;

	/* If a new string is provided, update the last token */
	if (str != NULL)
	{
		lastToken = str;
	}
	else
	{
		/* If the last token is NULL, we've reached the end of the string */
		if (lastToken == NULL)
		{
			return NULL;
		}
	}

	/* Find the first occurrence of any delimiter in the remaining string */
	while (*lastToken != '\0' && strchr(delimiters, *lastToken) != NULL)
	{
		lastToken++;
	}

	/* If we reached the end of the string, no more tokens */
	if (*lastToken == '\0')
	{
		lastToken = NULL; /* Reset for subsequent calls */
		return NULL;
	}

	/* Find the end of the token */
	tokenStart = lastToken;
	while (*lastToken != '\0' && strchr(delimiters, *lastToken) == NULL)
	{
		lastToken++;
	}

	/* If we reached the end of the string, update the last token and return the current token */
	if (*lastToken == '\0')
	{
		lastToken = NULL; /* Reset for subsequent calls */
	}
	else
	{
		*lastToken = '\0'; /* Null-terminate the current token */
		lastToken++;
	}

	return tokenStart;
}
/**
 * tokenize - Tokenizes a string into an array of command arguments.
 * @input: Input string to be tokenized.
 * @command: Array to store the tokenized command arguments.
 * @max_args: Maximum number of arguments in the array.
 *
 * Description: Uses strtok to split the input string into tokens
 * separated by spaces. Stores the tokens in the command array.
 */
void tokenize(char *input, char *command[], int max_args)
{
	char *token = str_tok(input, ";");
	int i = 0;
	char *tokenized_input = strdup(input);
	/*Create a copy of the input*/
	if (tokenized_input == NULL)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	while (token != NULL && i < max_args)
	{
		command[i++] = token;
		token = str_tok(NULL, ";");
	}
	command[i] = NULL;/* Null-terminate the commands array*/
	free(tokenized_input);/* Free the allocated memory for the copy*/
}
