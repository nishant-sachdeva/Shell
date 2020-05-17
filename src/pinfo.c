#include "main.h"


void pinfo_function(char ** argv, int args)
{
    // printf("pinfo function mein  hain\n");
    char *id = (char*)malloc(1024*sizeof(char));
    if (args == 1)
    {
        sprintf(id, "%d", getpid());
    }
    else 
    {
        id = argv[1];
    }

    // okay, so I have my id to work with

    // make a /proc/pid/exe   and  /proc/pid/status
    //  status wala thing is a file, read it's contents

    char path_exe[1024];
    char path_status[1024];

    strcpy(path_status, "/proc/");
    strcat(path_status, id);

    strcpy(path_exe, path_status);

    strcat(path_exe, "/cwd");
    strcat(path_status, "/status");

    //  abi we have 2 file paths, jhaan seh data read karna hai
    char *data =  malloc(1024*sizeof(char));    
    FILE* stats = fopen(path_status,  "r"); // it's a readonly file
    size_t size = 1024;
    // in the files, we want field, 1 (pid), 3 (proc status), 23(mem)
    int field = 0;
    while(field < 24)
    {
        if(getline(&data, &size, stats) == -1)
        {
            perror("Oops! Error occurred! Please check\n");
            break;
        }
        if(field == 1)
        {
            printf("Process id -- %s\n", id);
        }
        else if(field == 2)
        {
            printf("Status info -- %s\n", data);
        }
        else if(field == 22)
        {
            printf("Virtual Memory %s\n", data);
        }
        // so these will be my required data fields
        field++;
    }
    fclose(stats);

    readlink(path_exe, data, 1024);

    printf("Executable Path -- %s\n", path_exe);

    return;
}