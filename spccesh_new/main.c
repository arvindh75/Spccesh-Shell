#include "headers.h"
#include "exit.h"
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "setname.h"
#include "prompt.h"
#include "exec_proc.h"
#include "pinfo.h"
#include "history.h"
#include "nightswatch.h"

#include "setenv.h"
#include "unsetenv.h"
#include "rdir.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void str_replace_main(char* target, const char* needle, const char* replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);
    while (1) {
        const char *p = strstr(tmp, needle);
        if (p == NULL) {
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

#define MAX_BUF_LEN 500

int main()
{
    int exit_loop = 0;
    char home_m[PATH_MAX];
    char hostname[HOST_NAME_MAX];  
    char username[LOGIN_NAME_MAX]; 
    char cwd[PATH_MAX];            
    char tcwd[PATH_MAX];           
    int len = 0;
    int c;
    int j;
    char* input = malloc(sizeof(char)*MAX_BUF_LEN);
    int num_args = 0;
    int exit_read = 0;
    int stdin_save = dup(0);
    int stdout_save = dup(1);
    char temp_rdir[100];
    char temp_rdir2[100];
    char args_rdir[100];
    char left[100];
    char right[100];
    char right2[100];
    int fd,fd2,fd3;
    int mulrd=0;
    int pip=0;
    int rd=0;
    int pipea[2];
    char inp_his[100];
    char** args = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); 
    if (getcwd(home_m, PATH_MAX) == NULL)
    {
        perror("getcwd()");
    }
    printf("\e[1;1H\e[2J"); // Clear Screen
    setname_f(username, hostname);
    while (exit_loop == 0)
    {
        len=0;
        num_args=0;
        exit_read=0;
        prompt_f(home_m, username, hostname, cwd, tcwd);
        while(exit_read == 0) {
            c = getchar();
            if(c != '\n') {
                input[len] = c;
                len++;
            }
            else {
                input[len] = '\0';
                len++;
                exit_read=1;
            }
        }
        str_replace_main(input,"&","&;");
        char* token = strtok(input, ";");
        while(token!=NULL) {
            args[num_args] = token;
            num_args++;
            token = strtok(NULL, ";");
        }
        for(j=0;j<num_args;j++) {
            rd=0;
            pip=0;
            fd=-1;
            fd2=-1;
            fd3=-1;
            mulrd=-1;
            if(args[j] != NULL) {
                for(int p=0;p<strlen(args[j]);p++) {
                    if(args[j][p] == '>' || args[j][p] == '<') {
                        rd=1;    
                    }
                    if(args[j][p] == '|') {
                        pip=1;
                    }
                }
                if(pip == 1) {
                    if(pipe(pipea) < 0) {
                        printf("Error piping.\n");
                        continue;
                    }
                    strcpy(args_rdir,args[j]);
                    strcpy(temp_rdir,args_rdir);
                    strcpy(temp_rdir2,args_rdir);
                    char* retp = strstr(args_rdir, "| ");
                    if(retp) {
                        str_replace_rdir(args_rdir, retp, "");
                        
                        if(dup2(pipea[1], STDOUT_FILENO) == -1) {
                            perror("Duplicating file descriptor.");
                            continue;
                        } 
                         
                        char* inp = strtok(args_rdir, " \t");
                        exec_proc_f(inp, home_m, username, hostname, cwd, tcwd);
                        dup2(stdout_save, STDOUT_FILENO);
                        close(pipea[1]);
                        
                        //char pipe_content[102];
                        //read(pipea[0], pipe_content, 100);
                        //printf("Contents of fildes[0]: \n%s\n\n", pipe_content);


                        if(dup2(pipea[0], STDIN_FILENO) == -1) {
                            perror("Duplicating file descriptor.");
                            continue;
                        }
                        //[char* xy[2];
                        //xy[0] = retp+2;
                        //xy[1]=NULL;
                        //printf("%s\n", xy[0]);
                        //if (execvp(xy[0], xy) == -1) {
                        //}    printf("Command not found.\n");
                        //}
                        char* inp2 = strtok(retp+2, " \t");
                        exec_proc_f(inp2, home_m, username, hostname, cwd, tcwd);
                        dup2(stdout_save, STDOUT_FILENO);
                        dup2(stdin_save, STDIN_FILENO);
                        close(pipea[0]);
                        //pip=0;
                    }
                }
                if(rd == 1) {
                    strcpy(inp_his,args[j]);
                    strcpy(args_rdir,args[j]);
                    strcpy(temp_rdir,args_rdir);
                    strcpy(temp_rdir2,args_rdir);
                    char* ret2 = strstr(args_rdir, "< ");
                    if(ret2) {
                        str_replace_rdir(temp_rdir2,ret2, "");
                        strcpy(left,temp_rdir2);
                        strcpy(right,ret2+2);
                        strcpy(args[j], left);

                        char* ret0 = strstr(args_rdir, ">> ");
                        if(ret0) {
                            //printf("RET:%s\n", ret+2);
                            //printf("TEM2:%s\n", temp2);
                            strcpy(right2,ret0+3);
                            mulrd=1;
                        }
                        else {
                            char *ret = strstr(args_rdir, "> ");
                            if(ret) {
                                //printf("RET:%s\n", ret+2);
                                //printf("TEM2:%s\n", temp2);
                                strcpy(right2,ret+2);
                                mulrd=0;
                            }
                        }
                        if(mulrd != -1){
                            str_replace_rdir(right, right2, "");
                            if(mulrd ==1)
                                str_replace_rdir(right," >> ", "");
                            else
                                str_replace_rdir(right, " > ", "");
                        }
                        fd = open(right, O_RDONLY);
                        if(fd < 0) {
                            printf("Cannot open file%s\n", right);
                            continue;
                        }
                        if(dup2(fd, STDIN_FILENO) == -1) {
                            perror("Duplicating file descriptor.");
                            continue;
                        }
                        if(mulrd == 1) {
                            fd2 = open(right2, O_WRONLY | O_CREAT | O_APPEND, 0644);
                            if(dup2(fd2, STDOUT_FILENO) == -1) {
                                perror("Duplicating file descriptor.");
                                continue;
                            }
                        }
                        else if (mulrd == 0) {
                            fd3 = open(right2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if(dup2(fd3, STDOUT_FILENO) == -1) {
                                perror("Duplicating file descriptor.");
                                continue;
                            }
                        }
                    }
                    else {
                        char* ret0 = strstr(args_rdir, ">> ");
                        if(ret0) {
                            str_replace_rdir(temp_rdir2,ret0, "");
                            //printf("RET:%s\n", ret+2);
                            //printf("TEM2:%s\n", temp2);
                            strcpy(left,temp_rdir2);
                            strcpy(right,ret0+3);
                            //printf("LEFT:%s\n", left);
                            //printf("RIGHT:%s\n", right);
                            fd = open(right, O_WRONLY | O_CREAT | O_APPEND, 0644);
                            if(dup2(fd, STDOUT_FILENO) == -1) {
                                perror("Duplicating file descriptor.");
                                continue;
                            }
                            strcpy(args[j], left);
                        }
                        else {
                            char* ret = strstr(args_rdir, "> ");
                            if(ret) {
                                str_replace_rdir(temp_rdir2,ret, "");
                                //printf("RET:%s\n", ret+2);
                                //printf("TEM2:%s\n", temp2);
                                strcpy(left,temp_rdir2);
                                strcpy(right,ret+2);
                                //printf("LEFT:%s\n", left);
                                //printf("RIGHT:%s\n", right);
                                fd = open(right, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                if(dup2(fd, STDOUT_FILENO) == -1) {
                                    perror("Duplicating file descriptor.");
                                    continue;
                                }
                                strcpy(args[j], left);
                            }
                        }
                    }

                    rd=0;
                    //rdir_f(args[j],home_m, cwd, tcwd);
                    add_his_f(home_m, inp_his, 0);
                }
                if(rd == 0 && pip == 0) {
                    strcpy(inp_his,args[j]);
                    char* inp = strtok(args[j], " \t");
                    if (inp != NULL)
                    {
                        if ((!strcmp(inp, "exit")) || (!strcmp(inp, "quit")))
                        {
                            exit_loop = exit_f();
                        }

                        else if (!strcmp(inp, "cd"))
                        {
                            cd_f(home_m);
                        }

                        else if (!strcmp(inp, "pwd"))
                        {
                            pwd_f(tcwd);
                        }

                        else if (!strcmp(inp, "echo"))
                        {
                            echo_f();
                        }

                        else if (!strcmp(inp, "ls"))
                        {
                            ls_f(home_m, cwd, tcwd);
                        }

                        else if (!strcmp(inp, "pinfo"))
                        {
                            pinfo_f(inp, home_m);
                        }

                        else if (!strcmp(inp, "history"))
                        {
                            add_his_f(home_m, inp_his, 1);
                        }

                        else if (!strcmp(inp, "nightswatch"))
                        {
                            nw_f();
                        }

                        else if (!strcmp(inp, "setenv"))
                        {
                            setenv_f();
                        }

                        else if (!strcmp(inp, "unsetenv"))
                        {
                            unsetenv_f();
                        }

                        else if (!strcmp(inp, "jobs"))
                        {
                            jobs_f();
                        }

                        else if (!strcmp(inp, "overkill"))
                        {
                            overkill_f();
                        }

                        else if (!strcmp(inp, "kjob"))
                        {
                            kjob_f();
                        }

                        else
                        {
                            exec_proc_f(inp, home_m, username, hostname, cwd, tcwd);
                        }
                        if(fd==-1 && fd2==-1 && fd3==-1)
                            add_his_f(home_m, inp_his, 0);
                    }
                    dup2(stdin_save, STDIN_FILENO);
                    dup2(stdout_save, STDOUT_FILENO);
                    fflush(stdout);
                    if(fd != -1)
                        close(fd);
                    if(fd2 != -1)
                        close(fd2);
                    if(fd3 != -1)
                        close(fd3);
                }
            }
        }
        printf("\n");
    }
    return 0;
}
