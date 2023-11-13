#include "shell.h"

char *custom_getline()
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t chars_read;
	char buffer[BUFFER_SIZE];
	ssize_t buffer_index = 0;

	while (1)
	{
		/* Check if there are characters in the buffer*/
		if (buffer_index > 0)
		{
			/* Look for a newline character in the buffer*/
			char *newline_pos = memchr(buffer, '\n', buffer_index);
			if (newline_pos != NULL)
			{
				/* Calculate the length of the line*/
				line_size = newline_pos - buffer + 1;
				/* Allocate memory for the line*/
				line = realloc(line, line_size);
				if (line == NULL)
				{
					perror("realloc");
					exit(EXIT_FAILURE);
				}
				/* Copy the line from the buffer to the allocated memory*/
				strncpy(line, buffer, line_size);/* Move the remaining characters in the buffer to the beginning*/
				memmove(buffer, newline_pos + 1, buffer_index - line_size);
				buffer_index -= line_size;
				break; /* Return the line*/
			}
		}
		/* If the buffer is empty or does not contain a newline character, read more characters*/
		chars_read = read(STDIN_FILENO, buffer + buffer_index, BUFFER_SIZE - buffer_index);
		if (chars_read == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		else if (chars_read == 0 && line == NULL)
		{
			/*end of file without reading any characters, exit the shell*/
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		else if (chars_read == 0 || (chars_read == 1 && buffer[buffer_index] == '\n'))
		{
			/* End of file or newline character, return the line*/
			break;
		}
		buffer_index += chars_read;
		/* Resize the line buffer to accommodate more characters*/
		line = realloc(line, line_size + buffer_index);
		if (line == NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		/* Copy the buffer content to the line buffer*/
		strncpy(line + line_size, buffer, buffer_index);
		line_size += buffer_index;
		/* Reset the buffer index for the next iteration*/
		buffer_index = 0;
	}
	/* Remove the newline character from the end of the line*/
	if (line_size > 0 && line[line_size - 1] == '\n')
	{
		line[line_size - 1] = '\0';
	}
	return line;
	
}
