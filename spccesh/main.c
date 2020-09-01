#include "headers.h"
#include "exit.h"
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "setname.h"
#include "prompt.h"
//#include "str_util.h"

int main() 
{
    int exit_loop = 0;
    char* inp;
    char home_m[PATH_MAX];
    char hostname[HOST_NAME_MAX]; //
    char username[LOGIN_NAME_MAX]; //
    char cwd[PATH_MAX]; //
    char tcwd[PATH_MAX]; //
    //printf("here1\n");
    if(getcwd(home_m, PATH_MAX) == NULL) {
        perror("getcwd()");
    }
    //printf("Home main: %s\n", home_m);
    setname_f(username, hostname);
    while(exit_loop ==  0) {
        inp = prompt_f(home_m, username, hostname, cwd, tcwd);
        if(inp != NULL) {

            if(!strcmp(inp, "exit")) {
                exit_loop = exit_f();
            }

            else if(!strcmp(inp, "cd")) {
                cd_f(home_m);
            }

            else if(!strcmp(inp, "pwd")) {
                pwd_f(tcwd);
            }

            else if(!strcmp(inp, "echo")) {
                echo_f();
            }

            else if(!strcmp(inp, "ls")) {
                ls_f(home_m, cwd, tcwd);
            }
                       
            else {
                printf("Command not found !\n");
            }
         }
        printf("\n");
    }
    return 0;
}
