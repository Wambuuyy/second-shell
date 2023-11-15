#include "shell.h

/**
 *  * execute_command - Execute a command based on its type.
 *   * @command: Command to execute.
 *    */
void execute_command(const char *const *command)
{
	    if (strcmp(command[0], "cd") == 0)
		        {
				        execute_cd(command);
					    }
	        else if (strcmp(command[0], "env") == 0)
			    {
				            execute_env();
					        }
		    else if (strcmp(command[0], "setenv") == 0)
			        {
					        execute_setenv(command);
						    }
		        else if (strcmp(command[0], "unsetenv") == 0)
				    {
					            execute_unsetenv(command);
						        }
			    else
				        {
						        size_t command_length = 0;
							        while (command[command_length] != NULL)
									        {
											            ++command_length;
												            }

								        execute_external_command(command, command_length);
									    }
}
