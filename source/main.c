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
    extern exit;
    extern hostname;
    extern username;
    extern cwd;
    extern tcwd;
    extern result;
    extern home; 
    extern p;
    extern new;
    extern mydir;
    extern ldir;
    extern myfile;
    extern mystat;
    extern buf;
    extern maxlen;
    extern count;
    extern pass;
    extern args;
    extern temp;
    extern a_ls;
    extern l_ls;
    extern ls_dir;
    extern cnt_ls;
    extern tf; 
    extern gf;
    extern input;

    setname_f();

    while(exit ==  0) {
        prompt_f();
        if(inp != NULL) {

            if(!strcmp(inp, "exit")) {
                exit_f();
            }

            else if(!strcmp(inp, "cd")) {
                cd();
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
