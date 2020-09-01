#include "headers.h"
#include "exit.h"
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "setname.h"
#include "prompt.h"
#include "str_util.h"

int main() 
{
    int exit_loop = 0;
    char* inp;
    char* home;
    char hostname[HOST_NAME_MAX]; //
    char username[LOGIN_NAME_MAX]; //
    char cwd[PATH_MAX]; //
    char tcwd[PATH_MAX]; //
    printf("here1\n");
    setname_f(home, username, hostname);
    while(exit_loop ==  0) {
        inp = prompt_f(home, username, hostname, cwd, tcwd);
        if(inp != NULL) {

            if(!strcmp(inp, "exit")) {
                exit_loop = exit_f();
            }

            else if(!strcmp(inp, "cd")) {
                cd_f(home);
            }

            else if(!strcmp(inp, "pwd")) {
                pwd_f(tcwd);
            }

            else if(!strcmp(inp, "echo")) {
                echo_f();
            }

            else if(!strcmp(inp, "ls")) {
                ls_f(home, cwd, tcwd);
            }
                       
            else {
                printf("Command not found !\n");
            }
        }
        printf("\n");
    }
    return 0;
}
