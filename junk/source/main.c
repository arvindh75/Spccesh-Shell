#include "headers.h"
#include "exit.h"
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "setname.h"
#include "prompt.h"
#include "str_util.h"

#define LS_SIZE 25
#define BUF_SIZE 1024

int main() 
{
    setname_f();

    while(exit_loop ==  0) {
        prompt_f();
        if(inp != NULL) {

            if(!strcmp(inp, "exit")) {
                exit_f();
            }

            else if(!strcmp(inp, "cd")) {
                cd_f();
            }

            else if(!strcmp(inp, "pwd")) {
                pwd_f();
            }

            else if(!strcmp(inp, "echo")) {
                echo_f();
            }

            else if(!strcmp(inp, "ls")) {
                ls_f();
            }
                       
            else {
                printf("Command not found !\n");
            }
        }
        printf("\n");
    }
    return 0;
}
