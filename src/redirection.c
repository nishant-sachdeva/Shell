#include "main.h"


void diversion(int input, int output, int append, char ** argv)
{
    char * input_path, *output_path, *append_path;

    // we will check according to the various conditions as to what all to be doing
    if(input == 1)
    {
        if(argv[0][0] == '~')
        {
            input_path = malloc(1024*sizeof(char));
            strcpy(input_path, home_path);
            strcat(input_path, argv[0]+1);
            input_path[strlen(input_path)+1] = '\0';
        }
        else
        {
            input_path = malloc(1024*sizeof(char));
            get_path(input_path);
            strcat(input_path, "/");
            strcat(input_path, argv[0]);
            input_path[strlen(input_path) + 1] = '\0';
        }
        // so now we have the input path. now we will open it and then should that be successful, we will use the dup command to set the input
        int input_fd = open(input_path, O_RDONLY);

        if(input_fd == -1)
        {
            perror("input file could not be accessed\n");
            return;
        }
        else
        {
            // now that we are here, we know that the file has opened
            dup2(input_fd, STDIN_FILENO);
        }
        
    }
    if(output == 1)
    {
        
        if(argv[1][0] == '~')
        {
            output_path = malloc(1024*sizeof(char));
            strcpy(output_path, home_path);
            strcat(output_path, argv[1]+1);
            input_path[strlen(output_path)+1] = '\0';
        }
        else
        {
            output_path = malloc(1024*sizeof(char));
            get_path(output_path);
            strcat(output_path, "/");
            strcat(output_path, argv[1]);
            output_path[strlen(output_path) + 1] = '\0';
        }
        // so now we have the input path. now we will open it and then should that be successful, we will use the dup command to set the input
        int output_fd = open(output_path, O_RDWR | O_CREAT, 0644 );

        if(output_fd == -1)
        {
            perror("output file could not be accessed\n");
            return;
        }
        else
        {
            // now that we are here, we know that the file has opened
            dup2(output_fd, STDOUT_FILENO);
        }
    
    }
    if(append == 1)
    {
         if(argv[2][0] == '~')
        {
            append_path = malloc(1024*sizeof(char));
            strcpy(append_path, home_path);
            strcat(append_path, argv[2]+1);
            input_path[strlen(append_path)+1] = '\0';
        }
        else
        {
            append_path = malloc(1024*sizeof(char));
            get_path(append_path);
            strcat(append_path, "/");
            strcat(append_path, argv[2]);
            append_path[strlen(append_path) + 1] = '\0';
        }
        // so now we have the input path. now we will open it and then should that be successful, we will use the dup command to set the input
        int append_fd = open(append_path, O_RDWR | O_CREAT, 0777 );

        if(append_fd == -1)
        {
            perror("output file could not be accessed\n");
            return;
        }
        else
        {
            lseek(append_fd,1, SEEK_END);
            // now that we are here, we know that the file has opened
            dup2(append_fd, STDOUT_FILENO);
        }
    }
    return;
}
