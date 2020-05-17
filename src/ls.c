#include "main.h"


void ls_function(char ** argv, int args, char home_path[])
{
    char * folder = (char*)malloc(1024*sizeof(char));
    strcpy(folder, ".") ; // for defaults
    int lflag = 0, aflag = 0;
    // check for flags
    for(int i = 1 ; i<args ; i++)
    {
        if(strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "-al") == 0 || strcmp(argv[i], "-la") == 0)
        {
            lflag = 1;
        }
        if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "-al") == 0 || strcmp(argv[i], "-la") == 0)
        {
            aflag = 1;
        }
    }

    // flag checking ho gayi
    // now get the directory jispe ls lagana hai

    for(int i = 1 ; i<args ; i++)
    {
        if(argv[i][0] == '-')
            continue ;  // yeh flag  variable  hai
        // now we reach here means that flag variable nai directory hai
        if(argv[i][0] == '~')
        {
            folder = malloc(1024*sizeof(char));
            strcpy(folder, home_path);
            strcat(folder, argv[i]+1);
            folder[strlen(folder)+1] = '\0';
        }
        else
        {
            // aageh ka folder hai, puura path needed
            get_path(folder);
            strcat(folder, "/");
            strcat(folder, argv[i]);
            folder[strlen(folder) + 1] = '\0';
        }
    }

    // agar koi bhi folder nai thaa, then  it'll still be the default value of .
    // abi folder khol ke uske contents nikal

    struct dirent ** list;
    int number_of_files = scandir(folder, &list, NULL, alphasort);
    if(number_of_files < 0)
    {
        perror("ERROR : ls failed!");
        return ;
    }
    else
    {
        for(int i = 0 ; i<number_of_files ; i++)
        {
            char *path,  *name = list[i]->d_name; // that's the  name
            // abi we will list file names and according to flags we will modify output
            if(aflag == 0)
            {
                if(name[0] == '.')
                continue;
            }
            if(lflag == 1)
            {

                // yhaan we will run struct
                struct stat data;
                char file[1024];
                strcpy(file,folder);
                strcat(file, "/");
                strcat(file, name);
                stat(file, &data);
                // printf("file path is %s\n", file);
                // now we have the details
                // now  we will read permissions just as we did in ass1
                char *permissions = (char *)malloc(1024*sizeof(char));
                strcpy(permissions, "----------");
                // 10 fields for the first thing
                if(S_ISDIR(data.st_mode))
                    permissions[0] = 'd';
                if(data.st_mode & S_IRUSR)
                    permissions[1] = 'r';
                if(data.st_mode & S_IWUSR)
                    permissions[2] = 'w';
                if(data.st_mode & S_IXUSR)
                    permissions[3] = 'x';
                if(data.st_mode & S_IRGRP)
                    permissions[4] = 'r';
                if(data.st_mode & S_IWGRP)
                    permissions[5] = 'w';
                if(data.st_mode & S_IXGRP)
                    permissions[6] = 'x';
                if(data.st_mode & S_IROTH)
                    permissions[7] = 'r';
                if(data.st_mode & S_IWOTH)
                    permissions[8] = 'w';
                if(data.st_mode & S_IXOTH)
                    permissions[9] = 'x';
                
                // that gives me the permissions 
                // now I need to get the user name
                char user[1024], group[1024];
                if(data.st_uid == 1000)
                {
                    getlogin_r(user, sizeof(user));
                }
                else if(data.st_uid == 0)
                {
                    strcpy(user,"root");
                }

                if(data.st_gid == 1000)
                {
                    getlogin_r(group, sizeof(group));
                }

                // now we will get the time details
                char * times = (char*)malloc(50*sizeof(char));
                times = ctime(&data.st_atime);
                
                // now we will get size details
                long long int size = data.st_size;

                // okay, so now we have all we need

                printf("%s %s %s %lld %s %s\n", permissions, user, group, size, times, name);
            }
            else
            {
                printf("%s   ", name);
            }
        }
        printf("\n");
        free(list);

    }
    
    return ;
}
