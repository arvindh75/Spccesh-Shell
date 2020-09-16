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
            if(args[j] != NULL) {
                char inp_his[100];
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

                    else
                    {
                        exec_proc_f(inp, home_m, username, hostname, cwd, tcwd);
                    }
                    
                   add_his_f(home_m, inp_his, 0);
                }
            }
        }
        printf("\n");
    }
    return 0;
}
