#include "headers.h"
#include "exec_proc.h"
#include "prompt.h"
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#define MAX_BG 100

char* home_t;

char* username_t;
char* hostname_t; 

char* cwd_t;
char* tcwd_t;
int shell_id;
char cur_com[100];
char scu[100];
int end_cnt=0;

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
        prompt_f(home_t, username_t, hostname_t, cwd_t, tcwd_t, scu);
        fflush(stdout);
    }
    else {
        procs[index].over=1;
        fprintf(stderr,"\033[1;31m");
        fprintf(stderr, "\n%s with pid [%d] exited abnormally.\n",name, pid);
        fprintf(stderr,"\033[0m");
        fprintf(stderr,"\n");
        prompt_f(home_t, username_t, hostname_t, cwd_t, tcwd_t, scu);
        fflush(stdout);
    }
    end_cnt+=1;
    return;
}

void jobs_f(char* suc) {
    int count=0;
    char fpath[100];
    char tpath[100];
    char str[25];
    char buff[1000];
    char status[100];
    FILE* f;
    int x;
    for(int i=0;i<MAX_BG;i++) {
        if(procs[i].over != 0) {
            x=1;
            sprintf(fpath, "/proc/%d",procs[i].pid);
            strcpy(tpath, fpath);

            strcat(tpath, "/stat");
            f = fopen(tpath, "r");
            if(f == NULL) {
                //perror("\npinfo ");
                continue;
            }
            while(x <= 3) {
                fscanf(f,"%s",buff);
                if(x == 3)
                    strcpy(status, buff);
                x++;
            }
            if(status[1] == 'S'){
                count++;
                strcpy(str,"Running");
                printf("[%d] %s %s [%d]\n",count, str, procs[i].name, procs[i].pid);
            }
            if(status[0] == 'T'){
                count++;
                strcpy(str,"Stopped");
                printf("[%d] %s %s [%d]\n",count, str, procs[i].name, procs[i].pid);
            }
            fclose(f);
        }
    }
}

void overkill_f(char* suc) {
    for(int i=0;i<MAX_BG; i++) {
        if(procs[i].over == -1) {
            kill(procs[i].pid, 9);
        }
    }
}

void contbg_f(char* suc) {
    char* temp="";
    temp = strtok(NULL, " \t");
    int pidbg = atoi(temp) + end_cnt;
    if(procs[pidbg-1].over == 0) {
        printf("Job not found.\n");
        strcpy(suc,"f");
        return;
    }
    kill(procs[pidbg-1].pid, 25);
    procs[pidbg-1].over = -1;
}

void bgfg_f(char* suc) {
    char* temp="";
    temp = strtok(NULL, " \t");
    for(int i=0;i<10;i++) {
        printf("%d %s %d %d\n",i, procs[i].name, procs[i].pid, procs[i].over);
    }
    int pidbg = atoi(temp) + end_cnt;
    int restpid = getpgid(getpid());
    if(procs[pidbg-1].over == 0) {
        printf("Job not found.\n");
        strcpy(suc,"f");
        return;
    }
    //fprintf(stderr, "CHILD PID: %d\n", procs[pidbg-1].pid);
    //fprintf(stderr, "CHILD GROUP PID: %d\n", getpgid(procs[pidbg-1].pid));
    int status;
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, getpgid(procs[pidbg-1].pid)) == -1) {
        perror("tcsetpgrp");
        strcpy(suc,"f");
    }
    //if (!tcsetpgrp(STDOUT_FILENO, getpgid(procs[pidbg-1].pid))) {
    //perror("tcsetpgrp stdout");
    //}
    kill(procs[pidbg-1].pid, SIGCONT);
    waitpid(procs[pidbg-1].pid, &status, WUNTRACED);
    if (WIFSTOPPED(status)) //&& WIFSIGNALED(status))
    {
        //printf("HERE\n");
        strcpy(suc,"f");
    } 
    if(WIFSIGNALED(status)) {
        strcpy(suc,"f");
        end_cnt++;
    }
    if (WIFEXITED(status)) {
        int es = WEXITSTATUS(status);
        if(es == 1) 
            strcpy(suc,"f");
    }
    if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1) {
        perror("tcsetpgrp");
        strcpy(suc,"f");
    }
    //if (!tcsetpgrp(STDOUT_FILENO, getpgrp())) {
    //perror("tcsetpgrp stdout");
    //}
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
}

void ctrlc(int num) {
    //fprintf(stderr,"%s\n","here ctrlc");
    if(!tcsetpgrp(STDIN_FILENO, getpgid(shell_id))) {
        perror("Restoring STDIN");
    }
    if(!tcsetpgrp(STDOUT_FILENO, getpgid(shell_id))) {
        perror("Restoring STDOUT");
    }
    kill(getpid(), 9);
}

void ctrlz(int num) {
    setpgid(shell_id, shell_id);
    fprintf(stderr,"%s\n","here ctrlc");
    if(!tcsetpgrp(STDIN_FILENO, getpgid(shell_id))) {
        //perror("Restoring STDIN");
    }
    if(!tcsetpgrp(STDOUT_FILENO, getpgid(shell_id))) {
        //perror("Restoring STDOUT");
    }
    procs[proc_count].pid = getpid();
    procs[proc_count].over = 1;
    strcpy(procs[proc_count].name,cur_com);
    proc_count++;
    kill(getpid(), 24);
    //fprintf(stderr,"%s\n","here ctrlc");
}

void kjob_f(char* suc) {
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
                strcpy(suc,"f");
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
        strcpy(suc,"f");
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

void exec_proc_f(char *inp, char *home, char* username, char* hostname, char* cwd, char* tcwd, char* suc)
{
    strcpy(scu,suc);
    shell_id = getpid();
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
        strcpy(suc,"f");
    }
    else
    {
        strcpy(cur_com, c_args[0]);
        if(bg == 1) {
            int forkret = fork();
            if (forkret == 0)
            {
                setpgid(0,0);
                signal(SIGTSTP, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                signal(SIGSTOP, SIG_DFL);
                if(execvp(c_args[0], c_args) == -1) {
                    printf("\nCommand not found!\n");
                    strcpy(suc,"f");
                    exit(EXIT_FAILURE);
                }
                //exit(EXIT_SUCCESS);
            }
            else {
                // MIGHT BE NECESSARY
                setpgid(forkret,forkret);
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
                setpgid(0,0);
                //signal(SIGTTOU, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                signal(SIGSTOP, SIG_DFL);
                //signal(SIGTTIN, SIG_DFL);
                //     signal(SIGINT, ctrlc);
                //     signal(SIGTSTP, ctrlz);
                //fprintf(stderr, "%s %d %d %d\n", "HERE", getpid(), getpgrp(), shell_id);
                if(execvp(c_args[0], c_args) == -1) {
                    printf("\nCommand not found!\n");
                    strcpy(suc,"f");
                    exit(1);
                }
            }
            else
            {
                int status;
                setpgid(forkret,forkret);
                signal(SIGTTOU, SIG_IGN);
                signal(SIGTTIN, SIG_IGN);
                if (tcsetpgrp(STDIN_FILENO, forkret) == -1) {
                    strcpy(suc,"f");
                    perror("tcsetpgrp");
                }
                //if (!tcsetpgrp(STDOUT_FILENO, forkret)) {
                //perror("tcsetpgrp");
                //}
                //if (!tcsetpgrp(STDOUT_FILENO, getpid())) {
                //perror("tcsetpgrp stdout");
                //}
                waitpid(forkret, &status, WUNTRACED);
                if (WIFSTOPPED(status)) //&& WIFSIGNALED(status))
                {
                    //printf("HERE\n");
                    procs[proc_count].pid = forkret;
                    procs[proc_count].over = 1;
                    strcpy(procs[proc_count].name,cur_com);
                    proc_count++;
                    strcpy(suc,"f");
                } 
                if(WIFSIGNALED(status)) {
                    strcpy(suc,"f");
                }
                if (WIFEXITED(status)) {
                    int es = WEXITSTATUS(status);
                    if(es == 1) 
                        strcpy(suc,"f");
                }
                //while (!WIFEXITED(status) && !WIFSIGNALED(status))
                //{
                //    waitpid(forkret, &status, WUNTRACED);
                //} 

                if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1) {
                    perror("tcsetpgrp");
                    strcpy(suc,"f");
                }
                signal(SIGTTOU, SIG_DFL);
                signal(SIGTTIN, SIG_DFL);
                //if (!tcsetpgrp(STDOUT_FILENO, getpgrp())) {
                //perror("tcsetpgrp");
                //}
                fflush(stdin);
                fflush(stdout);
                fflush(stderr);
            }
        }
    }
}
