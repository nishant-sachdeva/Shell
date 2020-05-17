#include "main.h"

void pwd_function(int background, char home_path[])
{
        char paath[1024];
        long int i = (long int)getcwd(paath, 1024);
        fill_path(paath, home_path);
        // here we will get path relative to our path
        if(!i)
        {
            perror("");
        }
        else
        {
            printf("%s\n" , paath);
        }
        // yeh child process aaya hai, woh exit jayega
}