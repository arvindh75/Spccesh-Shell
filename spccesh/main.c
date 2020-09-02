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
#define MAX_BUF_LEN 500
//#include "str_util.h"


char* read_cmdline() {
    int len=0,c;            
    char* cmd = malloc(sizeof(char)*MAX_BUF_LEN);
    while(1) {
        c = getchar();
        if(c == '\n') {
            cmd[len++] = '\0';
            break;
        }
        else
            cmd[len++] = c;
    }
    return cmd;
}

int parse_cmd_line(char* cmdline, char** cmds) {
    int num_cmds = 0;
    char* token = strtok(cmdline, ";");
    while(token!=NULL) {
        cmds[num_cmds++] = token;
        token = strtok(NULL, ";");
    }
    return num_cmds;
}

int main()
{
    int exit_loop = 0;
    char *inp;
    char home_m[PATH_MAX];
    char hostname[HOST_NAME_MAX];  //
    char username[LOGIN_NAME_MAX]; //
    char cwd[PATH_MAX];            //
    char tcwd[PATH_MAX];           //
    char input[500];
    int len=0,c;
    char* temp = "";
    char args[LS_SIZE][1000];
    //printf("here1\n");
    if (getcwd(home_m, PATH_MAX) == NULL)
    {
        perror("getcwd()");
    }
    //printf("Home main: %s\n", home_m);
    setname_f(username, hostname);
    while (exit_loop == 0)
    {
        prompt_f(home_m, username, hostname, cwd, tcwd);
        int i,j;

        char** cmds = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); // array of semi-colon separated commands

        for(j = 0; j < MAX_BUF_LEN; j++) cmds[j] = '\0';

        char* cmdline = read_cmdline(); // read command line
        int num_cmds = parse_cmd_line(cmdline, cmds); // parse command line

        for(int j=0;j<num_cmds;j++) {
            if(cmds[j] != NULL) {
                char* inp = strtok(cmds[j], " \t");
                if (inp != NULL)
                {

                    if (!strcmp(inp, "exit"))
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

                    else
                    {
                        exec_proc_f(inp, home_m);
                    }
                }
            }
        }
        printf("\n");
    }
    return 0;
}
