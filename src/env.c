#include "main.h"

void set_env(char ** argv, int arg)
{
    // this is the function for setting (and if it dosen't exist, then creating ) the env variables as per specifications

    if(arg == 1 || arg > 3)
    {
        printf("There is an error in the arguments passed. Please check with the readme and try again :)\n");
        return;
    }

    if(arg == 2)
    {
        if(setenv(argv[1], "", 1) == -1){
            perror("");
        }
        return;
    }

    //argv[1] is the varname and argv[2] is the var value

    // now we will begin the checking and working
    if(setenv(argv[1], argv[2], 1) == -1)
    {
        perror("");
        return;
    }
    else
    {
        // all well , simply return;
    }
    
    return;
}



void unset_env(char ** argv, int arg)
{
    // now we are trying to unset arguments here, 
    if(arg != 2)
    {
        printf("There is an error in the number of arguments, please consult the readme and try again\n");
        return;
    }
    // now that we have the correct number of arguments
    if(getenv(argv[1]))
    {
        // means that the thing exists, so deleting it makes sense
        if(unsetenv(argv[1]) == -1)
        {
            perror("");
            return;
        }
    }
    else
    {
        printf("specified variable dosen't exist, looks like the job dosen't need to be performed :)\n");
        return;
    }
    
    return;
}