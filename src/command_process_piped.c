#include "main.h"


void do_work(char inp[], char home_path[])
{
	int val = check_for_up_arrow_keys(inp , home_path);
	if(val == -1)
		return;
	else if(val == 0)
	{
		// we have the desired command, print it and then execute it
		printf("\n");
		displayPrompt(home_path);
		printf("%s\n", inp);
	}
	else if(val == 4)
	{
		// printf("value returned is 4\n");
		// do nothig
	}
	add_to_history(inp, home_path);

	
	// now we can go on with the execution
	
	
	char *secondary;
	char *command = strtok_r(inp, ";", &secondary);
	// step 1, semicolon separation
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDIN_FILENO);
		
	while (command != NULL)
	{
		dup2(saved_stdout, 1);
		close(saved_stdout);

		dup2(saved_stdin, 0);
		close(saved_stdin);

		// check for up arrow keys


		char * secondary_pipe;

		char * piped_command = strtok_r(command, "|", &secondary_pipe);


		int input_pipe = saved_stdin;
		// first command takes input from stdin

		while(piped_command != NULL)
		{
			char *secondary_2, *copy_of_command;

			copy_of_command = (char *)malloc((1000)*sizeof(char));
			strcpy(copy_of_command, piped_command);
			// copy of the command created to send to commands like echo etc which will be lost in the strtok operations

			char *parts = strtok_r(piped_command, " ", &secondary_2);

			char * input, *output, *append;
			int arg = 0;
			char * argv[100];
			int background_required = 0;

			output = malloc(1024*sizeof(char));
			input = malloc(1024*sizeof(char));
			append = malloc(1024*sizeof(char));

			int input_redirection = 0, output_redirection = 0, append_output = 0;
			
			while (parts != NULL)
			{
				add_to_foreground(0);

				if(strcmp(parts, "quit") == 0 || strcmp(parts,"quit\n")== 0)
				{
					exit(0);
				}

				if(strcmp(parts, "&") == 0 || strcmp(parts, "&\n") == 0)
				{
					background_required = 1;
					parts = strtok_r(NULL, " ", &secondary_2);
					continue; 
					// so we basically ignore the background symbol to provide for further commands after the background symbol
				}
				// mark background as 1 if required

				if(strcmp(parts, ">") == 0 || strcmp(parts , ">\n")== 0)
				{
					output_redirection = 1;
					parts = strtok_r(NULL, " ", &secondary_2);
					if(parts[strlen(parts)-1] =='\n')
						parts[strlen(parts)-1] = '\0';
					strcpy(output , parts);
					parts = strtok_r(NULL, " ", &secondary_2);
					continue;
				}
				if(strcmp(parts, ">>") == 0 || strcmp(parts , ">>\n")== 0)
				{
					append_output = 1;
					parts = strtok_r(NULL, " ", &secondary_2);
					if(parts[strlen(parts)-1] =='\n')
						parts[strlen(parts)-1] = '\0';
					strcpy(append , parts);
					parts = strtok_r(NULL, " ", &secondary_2);
					continue;
				}
				if(strcmp(parts, "<") == 0 || strcmp(parts,"<\n") == 0)
				{
					input_redirection = 1;
					parts = strtok_r(NULL, " ", &secondary_2);
					if(parts[strlen(parts)-1] =='\n')
						parts[strlen(parts)-1] = '\0';
					strcpy(input, parts);
					parts = strtok_r(NULL, " ", &secondary_2);
					continue;
				}
				// so by the time I reach here , we are past the point where there are parameters for the commands
				if(strcmp(parts, "\n") !=0 )
				{
					argv[arg]=(char *)malloc((100)*sizeof(char));
					if(parts[strlen(parts)-1] =='\n')
						parts[strlen(parts)-1] = '\0';
					strcpy(argv[arg], parts);
					arg++;
				}
				parts = strtok_r(NULL, " ", &secondary_2);
			}

			argv[arg]=(char *)malloc((100)*sizeof(char));
			argv[arg]  = NULL;

			char * redirect[5];
			if(input_redirection == 1)
			{
				// printf("input is %s\n", input);
				redirect[0] = (char*)malloc((100)*sizeof(char));
				strcpy(redirect[0], input);
			}
			else
			{
				redirect[0] = (char*)malloc((100)*sizeof(char));
				redirect[0] = NULL;
			}
			if(output_redirection == 1)
			{
				// printf("output is %s\n", output);
				redirect[1] = (char*)malloc((100)*sizeof(char));
				strcpy(redirect[1], output);
			}
			else
			{
				redirect[1] = (char*)malloc((100)*sizeof(char));
				redirect[1] = NULL;
			}

			if(append_output == 1)
			{
				// printf("output is %s\n", append);
				redirect[2] = (char*)malloc((100)*sizeof(char));
				strcpy(redirect[2], append);
			}
			else
			{
				redirect[2] = (char*)malloc((100)*sizeof(char));
				redirect[2] = NULL;
			}

			// we have the whole command with a single process here, 
			// since we do not want piping/redirection to happen to the system commands, 
			// we will execute them here itself

			if(arg != 0)
			{
			if(!strcmp(argv[0] , "cd") || !strcmp(argv[0], "cd\n"))
			{
				cd_function(argv,arg,home_path);
				piped_command = strtok_r(NULL, "|", &secondary_pipe);
				continue;
			}
			else if(!strcmp(argv[0] , "fg") || !strcmp(argv[0], "fg\n"))
			{
				fg_function(argv, arg);
				piped_command = strtok_r(NULL, "|", &secondary_pipe);
				continue;
			}
			else if(!strcmp(argv[0] , "bg") || !strcmp(argv[0], "bg\n"))
			{
				bg_function(argv, arg);
				piped_command = strtok_r(NULL, "|", &secondary_pipe);
				continue;
			}
			else if(!strcmp(argv[0] , "setenv") || !strcmp(argv[0], "setenv\n"))
			{
				set_env(argv, arg);
				piped_command = strtok_r(NULL, "|", &secondary_pipe);
				continue;
			}
			else if(!strcmp(argv[0] , "unsetenv") || !strcmp(argv[0], "unsetenv\n"))
			{
				unset_env(argv, arg);
				piped_command = strtok_r(NULL, "|", &secondary_pipe);
				continue;
			}
			}



			int fd[2] ; // in/out pipe ends
			pipe(fd);  // pipe has been created fd[0] is the read end and fd[1] is the write end

			pid_t pid;
			int status;
			pid = fork();
			if(pid < 0)
			{
				printf("forking failed\n");
			}

			if(pid == 0)
			{
				if(strcmp(secondary_pipe, "\0") == 0)
				{
					close(fd[0]);
					dup2(input_pipe,STDIN_FILENO);
					close(input_pipe);
					// dup2(STDOUT_FILENO, STDOUT_FILENO);
				}
				else
				{
					// this is for the regular piped commands
					close(fd[0]); // close unused read end of the pipe
					dup2(input_pipe, STDIN_FILENO);
					close(input_pipe);
					dup2(fd[1], STDOUT_FILENO);
					close(fd[1]);
				}

				diversion(input_redirection, output_redirection, append_output, redirect);

				// here we check for the command being made of up arrow  keys and deal with it accordingly
				

				if(arg != 0) // cuz null seh comparison gives us a seg fault
				{
					if(!strcmp(argv[0] , "ls") || !strcmp(argv[0], "ls\n"))
					{
						ls_function(argv, arg, home_path);
					}
					else if(!strcmp(argv[0] , "pwd") || !strcmp(argv[0], "pwd\n"))
					{
						pwd_function(background_required, home_path);
					}
					else if(!strcmp(argv[0] ,"pinfo") || !strcmp(argv[0],"pinfo\n"))
					{
						pinfo_function(argv, arg);				
					}
					else if(!strcmp(argv[0] ,"history")||!strcmp(argv[0], "history\n"))
					{
						history_function(argv, home_path , arg);
					}
					else if(!strcmp(argv[0] , "echo") || !strcmp(argv[0], "echo\n"))
					{
						echo_function(copy_of_command);
					}				
					else if(!strcmp(argv[0] , "kjob") || !strcmp(argv[0], "kjob\n"))
					{
						kjobs(argv, arg);
					}
					else if(!strcmp(argv[0] , "jobs") || !strcmp(argv[0], "jobs\n"))
					{
						jobs_list(argv, arg);
					}
					else if(!strcmp(argv[0] , "overkill") || !strcmp(argv[0], "overkill\n"))
					{
						overkill_func(argv, arg);
					}
					else if(!strcmp(argv[0] , "fg") || !strcmp(argv[0], "fg\n"))
					{
						exit(0);
					}	
					else if(!strcmp(argv[0] , "bg") || !strcmp(argv[0], "bg\n"))
					{
						exit(0);
					}
					else
					{
						if(background_required == 1)
							setpgid(0,0);
						if( execvp(*argv, argv) < 0)
						{
							printf("Error : %s  failed!\n", argv[0]);
						}
					}
				}
				// we have to exit once we return from our respective procsses
				exit(0);
			}
			else
			{
				if(background_required == 0)
				{
					// means no background wanted
					add_to_foreground((int)pid);
					// (void)waitpid(pid, &status, 0);
					waitpid(pid, &status, WUNTRACED);
				}
				if(background_required == 1)
				{
					add_to_background((int)pid);
					// kill(pid, SIGTTOU);

					deal_with_background();

				}
				if(strcmp(secondary_pipe, "\0") != 0)
				{
					close(fd[1]) ; // closing unused write end of the pipe
					input_pipe = fd[0] ; // the next command reads from here
				 }
								
			}
			// now go to the next command
			// this step will take us to the next command 
			free(input);
			free(output);
			free(append);

			piped_command = strtok_r(NULL, "|", &secondary_pipe);
		}
		command = strtok_r(NULL, ";", &secondary);
	}
	return ;
}