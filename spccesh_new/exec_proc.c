#include "headers.h"
#include "exec_proc.h"
#include "prompt.h"
#include <termios.h>
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
        if(procs[i].over == -1) {
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
            fprintf(stderr, "\n%s with pid [%d] exited normally.\n",name, pid);
            fprintf(stderr,"\033[0m");
            fprintf(stderr,"\n");
        }
        else {
            procs[index].over=1;
            fprintf(stderr,"\033[1;31m");
            fprintf(stderr, "\n%s with pid [%d] exited abnormally.\n",name, pid);
            fprintf(stderr,"\033[0m");
            fprintf(stderr,"\n");
        }
        prompt_f(home_t, username_t, hostname_t, cwd_t, tcwd_t);
        fflush(stdout);
    }
    else {
        procs[index].over=1;
        fprintf(stderr,"\033[1;31m");
        fprintf(stderr, "\n%s with pid [%d] exited abnormally.\n",name, pid);
        fprintf(stderr,"\033[0m");
        fprintf(stderr,"\n");
        prompt_f(home_t, username_t, hostname_t, cwd_t, tcwd_t);
        fflush(stdout);
    }
    return;
}

void jobs_f() {
    int count=0;
    char str[25];
    for(int i=0;i<MAX_BG;i++) {
        if(procs[i].over == -1){
            count++;
            strcpy(str,"Running");
            printf("[%d] %s %s [%d]\n",count, str, procs[i].name, procs[i].pid);
        }
        if(procs[i].over == 1){
            count++;
            strcpy(str,"Stopped");
            printf("[%d] %s %s [%d]\n",count, str, procs[i].name, procs[i].pid);
        }
    }
}

void overkill_f() {
    for(int i=0;i<MAX_BG; i++) {
        if(procs[i].over == -1) {
            kill(procs[i].pid, 9);
        }
    }
}

void kjob_f() {
    char *temp = "";
    char args[LS_SIZE][COM_LEN];
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
    int count_args=0;
    int pid = -1;
    int signum = -1;
    for(int j=0;j<LS_SIZE; j++) {
        if(args[j][0] >= 48 && args[j][0] <=57) {
            if(count_args > 1) {
                printf("Wrong Arguments!\n");
                return;
            }
            if(count_args == 0) {
                pid = atoi(args[j]);
            }
            if(count_args == 1) {
                signum = atoi(args[j]);
            }
            count_args++;
        }
    }
    if(pid == -1 || signum == -1) {
        printf("Wrong Arguments!\n");
        return;
    }
    kill(pid, signum);
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
    pid_t par;
    tcwd_t = tcwd;
    char *c_args[LS_SIZE];
    char *temp = "";
    char args[LS_SIZE][COM_LEN];
    int count = 0;
    int bg = 0;
    if(inp[strlen(inp)-1] == 38) {
        bg=1;
        inp[strlen(inp)-1]='\0';
    }
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
    //fprintf(stderr,"\033[1;31m");
    //for(int g=0;g<count+1;g++)
    //    fprintf(stderr, "ARGS[%d]:%s\n",g,c_args[g]);
    // fprintf(stderr,"\033[0m");
    if (c_args[0] == NULL)
    {
        printf("\nCommand not found!\n");
    }
    else
    {
        if(bg == 1) {
            int forkret = fork();
            if (forkret == 0)
            {
                setpgid(0,0);
                if(execvp(c_args[0], c_args) == -1) {
                    printf("\nCommand not found!\n");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }
            else {
                // MIGHT BE NECESSARY
                //  par = getpid();
                //  setpgid(par,par);
                procs[proc_count].pid = forkret;
                procs[proc_count].over = -1;
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
                    printf("\nCommand not found!\n");
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
