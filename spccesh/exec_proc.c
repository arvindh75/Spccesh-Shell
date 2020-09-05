#include "headers.h"
#include "exec_proc.h"
#include "prompt.h"
#include <linux/limits.h>
#include <unistd.h>
#define MAX_BG 100

char* home_t;
char* username_t;
char* hostname_t; 
char* cwd_t;
char* tcwd_t;

struct bgp {
    int pid;
    int over;
    char name[32];
} procs[MAX_BG];

int proc_count =0;

void proc_end(int num) {
    int status;
    pid_t pid = waitpid(-1,&status, WNOHANG);
    if(pid <= 0)
        return;
    char name[100];
    int index;
    for(int i=0;i<MAX_BG;i++) {
        if(procs[i].over == 0) {
            if(procs[i].pid == pid) {
                strcpy(name, procs[i].name);
                index=i;
            }
        }
    }
    if(WIFEXITED(status)) {
        if(WEXITSTATUS(status) == EXIT_SUCCESS) {
            procs[index].over=1;
            fprintf(stderr,"\033[1;32m");
            fprintf(stderr, "\n%s with pid [%d] exited normally.\n\n",name, pid);
            fprintf(stderr,"\033[0m");
        }
        else {
            procs[index].over=1;
            fprintf(stderr,"\033[1;31m");
            fprintf(stderr, "\n%s with pid [%d] exited abnormally.\n\n",name, pid);
            fprintf(stderr,"\033[0m");
        }
        prompt_f(home_t, username_t, hostname_t, cwd_t, tcwd_t);
        fflush(stdout);
    }
    //prompt_f(home_t, username_t, hostname_t, cwd_t, tcwd_t);
    //fflush(stdout);
    return;
}

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

void exec_proc_f(char *inp, char *home, char* username, char* hostname, char* cwd, char* tcwd)
{
    home_t = home;
    username_t = username;
    hostname_t = hostname;
    cwd_t = cwd;
    tcwd_t = tcwd;
    char *c_args[LS_SIZE];
    char *temp = "";
    char args[LS_SIZE][1000];
    int count = 0;
    int bg = 0;
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
    int str_len=0;
    for (int j = 0; j < LS_SIZE; j++)
    {
        //        if (( (args[j][0] >= 48 && args[j][0] <= 57)|| (args[j][0] >= 97 && args[j][0] <= 122) || (args[j][0] >= 65 && args[j][0] <= 90) || (args[j][0] == 46) || (args[j][0] == 47) || (args[j][0] == 126) || (args[j][0] == 38) || args[j][0] == 45) && args[j] != NULL)
        if(args[j][0] != 0)
        {
            str_len = strlen(args[j]);
            if (args[j][0] == '~')
            {
                str_replace_ep(args[j], "~", home);
            }
            else {
                if(args[j][str_len-1] == 38) {
                    bg=1;
                    args[j][str_len-1]='\0';
                }
                else if(args[j][0] == 38) {
                    bg=1;
                }
                else {
                    c_args[count++] = args[j];
                }
            }
        }
    }
    c_args[count] = NULL;
    if (c_args[0] == NULL)
    {
        printf("Command not found !\n");
    }
    else
    {
        if(bg == 1) {
            int forkret = fork();
            if (forkret == 0)
            {
                setpgid(0,0);
                if(execvp(c_args[0], c_args) == -1) {
                    printf("Command not found !\n");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }
            else {
                procs[proc_count].pid = forkret;
                procs[proc_count].over = 0;
                strcpy(procs[proc_count].name, c_args[0]);
                proc_count++;
                printf("[%d]\n", forkret);
                signal(SIGCHLD, proc_end);
            }
        }
        else {
            int forkret = fork();
            if (forkret == 0)
            {
                if(execvp(c_args[0], c_args) == -1) {
                    printf("Command not found !\n");
                    exit(1);
                }
                exit(0);
            }
            else
            {
                int status;
                if (!tcsetpgrp(STDIN_FILENO, getpid())) {
                    //perror("tcsetpgrp");
                }
                if (!tcsetpgrp(STDOUT_FILENO, getpid())) {
                    //perror("tcsetpgrp stdout");
                }
                waitpid(forkret, &status, WUNTRACED);
                while (!WIFEXITED(status) && !WIFSIGNALED(status))
                {
                    waitpid(forkret, &status, WUNTRACED);
                } 
            }
        }
    }
}
