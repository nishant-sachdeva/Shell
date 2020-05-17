#include "main.h"

char command[100][100];

char home[1024];
char rep[1024];


int get_count(){

    FILE * lines = fopen(home, "r");
    //read file into array
    int count = 0;
    char c;
    for(c = getc(lines); c!= EOF ; c = getc(lines))
    {
        if(c == '\n')
            count++;
    }
    return count;
}


void history_function(char *argv[100], char home_path[], int arg)
{
    int number = 0;
    if(arg == 1)
        number = 10;
    else if(arg == 2)
        number = argv[1][0] - '0';
    update(home_path);
    FILE *myFile, *lines;
    myFile = fopen(home, "r");
    // printf("file open hui\n");
    int count = get_count();
    // printf("count is : %d\n", count);
    if(count <= number)
    {
        for (int i = 0; i < count ; i++)
        {
            fscanf(myFile, "%[^\n]",command[i]);
            char ch = getc(myFile);
            printf("%d : %s\n",i+1, command[i]);
        }
    }
    else
    {
        // assuming count  >= 10
        for(int i = 1 ; i<=count ; i++)
        {
            fscanf(myFile, "%[^\n]",command[i]);
            char ch = getc(myFile);
            // printf("count bigger than  1o  %d\n", count);
            if(i > (count-number))
            {
                printf("%d : %s\n",i-(count-number), command[i]);
            }            
        }
    }
    
    fclose(myFile);
    fclose(lines);
}



void add_to_history(char * command, char home_path[])
{

    update(home_path);

    FILE * file = fopen(home, "a");
    int count  = get_count();
    if((count) <= 19)
    {
        FILE * f = fopen(home, "a");
        fprintf(f, "%s", command);
        fclose(f);
    }
    else
    {
        FILE * f = fopen(home, "a");
        fprintf(f, "%s", command);
        fclose(f);

        FILE *  file1 = fopen(home, "r");
        FILE * file2  = fopen(rep, "w");
        int temp =  1;
        for(char c = getc(file1); c!= EOF ; c = getc(file1))
        {
            if(temp != 1)
            {
                putc(c, file2);
            }
            if(c == '\n')
                temp++;
        }
        fclose(file1);
        fclose(file2);
        

        remove(home);
        rename(rep, home);
        return ;
    }
    

}



void update(char home_path[])
{
    strcpy(home, home_path);
    strcat(home , "/");
    strcpy(rep , home);
    strcat(home, ".history");

    strcat(rep, ".replica");

    return;

}


int check_for_up_arrow_keys(char inp[] , char home_path[])
{
    update(home_path);
    if(inp[0] == '')
    {
        int count = 0;
        // means up arrow key hai
        for(int i = 0  ; i<strlen(inp); i++)
        {
            if(inp[i] == '')
            {
                count++;
            }
        }
       //  okay, now we have the count
        // printf("#%d count\n", count);
        if(count >= 10)
        {
            printf("invalid number of arrows. These number of commands are not stored\n");
            return -1;
        }
        // now that we are here, we know that it might be possible to get that command
        // so the idea is that I want the "count" number of command (if it exists)
            // printf("say hi\n");
       
        int number = get_count();
    //    printf("say hi\n");
        if(count > number)
        {
            printf("invalid number of arrows. These number of commands are not stored\n");
            return -1;            
        }
        else
        {
            // printf("%d\n",count );
            update(home_path);
            FILE *myFile, *lines;
            myFile = fopen(home, "r");
            // printf("file open hui\n");
            int want = 20 - count ;
            for (int i = 0; i < 20; i++)
            {
                // printf("say hi\n");
                fscanf(myFile, "%[^\n]",command[i]);
                char ch = getc(myFile);
                // printf("command number %d is %s\n", i, command[i]);
                if(i == want)
                {
                    //printf("%d : %s\n",i+1, command[i]);
                    sprintf(inp, "%s\n", command[i]);
                    // printf("say hi plis%s\n",inp );
                    break;
                }
                //  this has the command, now  we can exit in peace
            }
        }
        return 0;
    }
    return 4;
}