#include "headers.h"
#include "exec_proc.h"

void str_replace_ep(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = {0};
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);
    while (1)
    {
        const char *p = strstr(tmp, needle);
        if (p == NULL)
        {
            strcpy(insert_point, tmp);
            break;
        }
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;
        tmp = p + needle_len;
    }
    strcpy(target, buffer);
}

void exec_proc_f(char *inp, char *home)
{
    char *c_args[LS_SIZE];
    char *temp = "";
    char args[LS_SIZE][1000];
    int count = 0;
    c_args[0] = inp;
    for (int j = 0; j < LS_SIZE; j++)
    {
        temp = strtok(NULL, " \t");
        if (temp == NULL)
        {
            for (int k = j; k < LS_SIZE; k++)
            {
                strcpy(args[k], "\0");
            }
            break;
        }
        int i = 0;
        for (i = 0; temp[i] != '\0'; i++)
        {
            args[j][i] = temp[i];
        }
        args[j][i] = '\0';
    }
    count = 1;
    for (int j = 0; j < LS_SIZE; j++)
    {
        if (((args[j][0] >= 97 && args[j][0] <= 122) || (args[j][0] >= 65 && args[j][0] <= 90) || (args[j][0] == 46) || (args[j][0] == 47) || (args[j][0] == 126) || (args[j][0] == 38)) && args[j] != NULL)
        {
            if (args[j][0] == '~')
            {
                str_replace_ep(args[j], "~", home);
            }
            //printf("Here: %s\n", args[j]);
            c_args[count++] = args[j];
        }
    }
    //printf("Command: %s\n", c_args[0]);
    //for(int k=0;k<count;k++) {
    //    printf("Arg: %s\n", c_args[k]);
    //}
    c_args[count] = NULL;
    if (c_args[0] == NULL)
    {
        printf("Command not found !\n");
    }
    else
    {
        if(!strcmp(c_args[count-1], "&")) {
            printf("BG process");
            //int forkret = fork();
            //if (forkret == 0)
            //{
            //    if(execvp(c_args[0], c_args) == -1) {
            //        printf("Command not found !\n");
            //    }
            //    exit(1);
            //}
        }
        else {
            int forkret = fork();
            if (forkret == 0)
            {
                if(execvp(c_args[0], c_args) == -1) {
                    printf("Command not found !\n");
                }
                exit(1);
            }
            else
            {
                wait(NULL);
            }
        }

        //  if (strcmp(c_args[count - 1], "&"))
        //  {
        //      if (forkret == 0)
        //      {
        //          execvp(c_args[0], c_args);
        //      }
        //      else
        //      {
        //          wait(NULL);
        //      }
        //  }
        //  else
        //  {
        //      printf("BG process\n");
        //  }
    }
}
