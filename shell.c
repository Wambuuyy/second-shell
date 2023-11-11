#include "shell.h"

int main() {
	    char input[MAX_COMMAND_LENGTH];

	        while (1) {
			        printf("Pru$ ");
				        if (fgets(input, sizeof(input), stdin) == NULL) {
						            if (feof(stdin)) {
								                    printf("\n");  /* Print a newline after Ctrl+D */
										                    break;  /* Exit the loop on Ctrl+D*/
												                }
							                perror("fgets");
									            exit(EXIT_FAILURE);
										            }

					        /* Remove the newline character*/
					        input[strcspn(input, "\n")] = '\0';

						        /* exit if the user enters "exit" or "quit"*/
						        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
								            break;
									            }

							        /* Execute the command*/
							        execute_command(input);
								    }

		    return 0;
}
