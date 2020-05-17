#include<stdio.h>
#include<unistd.h>
#include<stdlib.h> 
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>

#include <syslog.h>
#include<sys/wait.h>
#include <fcntl.h>  
#include <stdlib.h>
#include<dirent.h>
#include<time.h>


extern char home_path[1024];


void do_work(char inp[], char home_path[]);

void signal_handler(int signal_number);

void ctrlZhandler(int signal);

void ctrlChandler(int signal);

void send_to_Chandler(int signal);

void send_to_Zhandler(int signal);

int check_for_up_arrow_keys(char inp[], char home_path[]);

void deal_with_background();

void overkill_func();

void add_to_foreground(int process_id); 

void add_to_background(int process_id); 

int print_info(int process_id, int job_number);

void fg_function(char ** argv, int arg);

void bg_function(char ** argv, int arg);

void handle_piped_command(char * piped_command, int command_number, int is_last_command, int input_pipe);

void kjobs(char ** argv, int arg);

void jobs_list(char **argv, int arg);

void set_env(char **argv, int arg);

void unset_env(char ** argv, int arg);

void get_path(char arr[]);

void displayPrompt(char home_path[]);

void take_commands(char input[]);

void fill_path(char path[], char home_path[]);

void parse(char *line, char **argv);

int substring(char arr1[], char arr2[]);

void pwd_function(int background,  char home_path[]);

void echo_function(char *command);

void cd_function(char **argv, int arg, char arr[]);

void pinfo_function(char ** argv, int args);

void ls_function(char **argv , int args, char home_path[]);

void history_function(char **argv, char home_path[], int arg);

void add_to_history(char *  arr, char home_path[]);

void update(char home_path[]);

void diversion(int input, int output, int append, char ** argv);



int main();