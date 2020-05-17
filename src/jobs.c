#include "main.h"
#define decimal 10

extern int foreground_process_id;

int background_processes_array[2048];

int background_counter = 0;


void add_to_foreground(int process_id)
{
    foreground_process_id = process_id;
    return;
}

void send_to_Chandler(int signal)
{
    if(foreground_process_id != 0)
    {
        if(kill(foreground_process_id, SIGINT) == -1);
    }
    return;
}

void send_to_Zhandler(int signal_number)
{
    if(foreground_process_id != 0)
    {
        signal(SIGTTOU, SIG_IGN);
        printf("sending the stop signal\n");
        tcsetpgrp(0, getpid());
        kill(foreground_process_id, SIGTSTP);
		// setpgid(0,0);

       signal(SIGTSTP, SIG_IGN);
       background_processes_array[background_counter++] = foreground_process_id;

    }
    return;
}

void add_to_background(int process_id)
{
    background_processes_array[background_counter++] = process_id;
    return ;
}

int print_info(int process_id, int job_number)
{
    char newname[1024] = {0};
    char folder[1024] = {0};
    char* name = (char*)calloc(1024,sizeof(char));
    if(name)
    {
        sprintf(folder, "/proc/%d/cmdline",process_id);
        FILE* f = fopen(folder,"r");

        if(f!=NULL)
        {
            size_t size;
            size = fread(name, sizeof(char), 1024, f);
            for(int i = 0 ; i<sizeof(name) ;i++ )
            {
                if(name[i] == '\0')
                {
                    name[i] = ' ';
                }
            }
            sprintf(newname, "%s", name);
            if(size>0)
            {
                if('\n'==newname[size-1])
                    newname[size-1]='\0';
            }
            fclose(f);
        }
        else
        {
            return -1;
        }
    }


    char *arr =  malloc(1024*sizeof(char));    
    char *id =  malloc(1024*sizeof(char));    
    char *status =  malloc(1024*sizeof(char));    
    size_t size = 1024;

    char* stat_file = (char*)calloc(1024,sizeof(char));
    sprintf(stat_file, "/proc/%d/status", process_id);
    if(stat_file)
    {
        FILE * data = fopen(stat_file, "r");
        if(data!=NULL)
        {
            int field = 0;
            while(field < 4)
            { 
                if(getline(&arr, &size, data) == -1)
                {
                    perror("Oops! Error occurred! Please check\n");
                    break;
                }
                if(field == 0)
                {
                    strcpy(id, arr);
                }
                else if(field == 2)
                {
                    strcpy(status, arr);
                    // okay, i will try to fix this \n thingy
                    char * secondary;
                    char *parts = strtok_r(status, "\n", &secondary);
                    strcpy(status, parts);
                    char * secondary2;
                    parts = strtok_r(status," ", &secondary2);
                    parts = strtok_r(NULL, " ", &secondary2);
                    strcpy(status, parts);
                }
                field++;
            }
        }
        else
        {
            return -1;
        }
        fclose(data);        
    }
    char proc_id[100];
    sprintf(proc_id, "%d", process_id);
    printf("[%d] : %s %s [%s]\n",job_number, status , newname, proc_id);

    return 1;

}

void jobs_list(char ** argv , int arg)
{
    int counter = 1;
    for(int i = 0 ; i < (background_counter); i++)
    {
        int a = print_info(background_processes_array[i], counter); // this is the oldest -> newest order
        if(a > 0)
        {
            // mean it was a success
            counter++;
        }
    }
    return ;
}

void kjobs(char ** argv, int arg)
{
    // so argv[1] is the job number in string form
    // argv[2] is the signal number in string form
    int job_number = atoi(argv[1]) - 1;
    int signal = atoi(argv[2]);

    if(job_number > background_counter)
    {
        printf("Error: job dosen't exist\n");
        return;
    }
    // given the job number I want the second job that exists in my list
    int counter = 0, found_job = 0;
    for(int i = 0 ; i < background_counter; i++)
    {
        int job = background_processes_array[i];
        if(kill(job,0) != -1)
        {
            // printf("job number %d of id %d exists\n", i, job);
            if(counter == job_number)
            {
                // this is our jobs
                found_job = 1;
                job_number = i;
                break;
            }
            counter++;

            // printf("counter is  %d job number is %d\n", counter-1, job_number);
        }
        else
        {
            // printf("job number %d of id %d  does not exist\n", i, job);
        }
               
    }
            // printf("counter is  %d job number is %d\n", counter, job_number);

    int job = background_processes_array[job_number];

    if(kill(job,signal) == -1)
    {
        perror("");
    }
    else
    {
        // we are good to go, keep going
    }
    
    return ;
}

void overkill_func()
{
    for (int i = 0; i < background_counter; i++)
    {
        if(kill(background_processes_array[i], 0) !=-1)
        {
            if(kill(background_processes_array[i], 9) == -1)
            {
                printf("Couldn't kill %d\n", background_processes_array[background_counter]);
            }
        }
    }
    return;
}
void fg_function(char ** argv, int arg)
{
    
    int job_number = atoi(argv[1]) -1;
    if(job_number > background_counter)
    {
        printf("Error: job dosen't exist\n");
        return;
    }
    // given the job number I want the second job that exists in my list
    int counter = 0, found_job = 0;
    for(int i = 0 ; i < background_counter; i++)
    {
        int job = background_processes_array[i];
        if(kill(job,0) != -1)
        {
            if(counter == job_number)
            {
                // this is our jobs
                found_job = 1;
                job_number = i;
                break;
            }
            counter++;

        }
               
    }
    int job = background_processes_array[job_number];
    if(found_job != 1)
    {
        printf("Error: couldn't access the specefied job :(\n");
        return;
    }
    else
    {
        // yhaan we are in the parent process
        pid_t foreground = tcgetpgrp(0);
        foreground_process_id = job;
        if(__getpgid(job) != __getpgid(0))
        {
            tcsetpgrp(0,job);
        }
        else
        {
            tcsetpgrp(job, job);
        }
        
        kill(job, SIGCONT);
        int stat;
        waitpid(job, &stat , WUNTRACED);

        signal(SIGTTOU, SIG_IGN);
        // setpgid(getpid(), getpid());
        tcsetpgrp(0, getpid());
        signal(SIGTTOU, SIG_DFL);
    }
    return ;
}

void bg_function(char ** argv, int arg)
{
    int job_number = atoi(argv[1]) -1;
    if(job_number > background_counter)
    {
        printf("Error: job dosen't exist\n");
        return;
    }
    int counter = 0, found_job = 0;
    for(int i = 0 ; i < background_counter; i++)
    {
        int job = background_processes_array[i];
        if(kill(job,0) != -1)
        {
            if(counter == job_number)
            {
                // this is our jobs
                found_job = 1;
                job_number = i;
                break;
            }
            counter++;

        }
               
    }
    int job = background_processes_array[job_number];
    if(kill(job, 0) == -1)
    {
        printf("Error: couldn't access the specified job\n");
        return;
    }
    else
    {
        // printf("say hi");
        kill(job, SIGCONT);
    }
    return;
}