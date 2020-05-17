#include "main.h"

char prev_directory[1024];

char current_dir[1024];

int count = 0; // just for the first time it is being called

// the idea is that the first time it is called, we will definitely be at the home directory


void cd_function(char ** argv, int arg, char home_path[])
{
    // for(int i = 0; i < arg; i++)
    // {
    //     printf("%s\n", argv[arg]);
    // }
    if(count == 0)
    {
        getcwd(prev_directory, 1024);
        count++;
        // this will assign it the home directory
        // and we are not coming back to this function, so it's all cool
    }

    char *paath, *path;
    if(arg == 1)
    {
        path = home_path;
        path[strlen(path)+1] = '\0';
    }
    else
    {
        if(argv[1][0] == '~')
        {
            paath = malloc(1024*sizeof(char));
            strcpy(paath, home_path);
            strcpy(paath + strlen(paath), argv[1]+1);
            paath[strlen(paath)+1] = '\0';
            argv[1] = paath;
            path = argv[1];
        }
        else if(argv[1][0] == '-')
        {
            // then we know that we are gonna go to the prev directory
            path = malloc(1024*sizeof(char));
            strcpy(path, prev_directory);
            path[strlen(path)+1] = '\0';
        }
        
        else
        {
            path = argv[1];
            path[strlen(path) + 1] = '\0';
        }
    }
    // so what happens here is that this gets recorded
    // so that if this happens next time, we will have a proper directory to go back to;
    getcwd(current_dir, 1024);
    if(chdir(path) != 0)
        perror("");
    else
    {
        
        strcpy(prev_directory, current_dir);
    }
    
    char arr[1024];
    get_path(arr);
    return;
}
