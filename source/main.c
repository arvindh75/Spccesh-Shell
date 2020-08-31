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
    // extern int exit_loop;
    // extern char hostname[];
    // extern char username[];
    // extern char cwd[];
    // extern char tcwd[];
    // extern int result;
    // extern char* home; 
    // extern char* p;
    // extern char new[];
    // extern DIR* mydir;
    // extern DIR* ldir;
    // extern struct dirent *myfile;
    // extern struct stat mystat;
    // extern char buf[];
    // extern int maxlen;
    // extern int count;
    // extern int pass;
    // extern char args[][1000];
    // extern char* temp;
    // extern int a_ls;
    // extern int l_ls;
    // extern char* ls_dir;
    // extern int cnt_ls;
    // extern struct passwd* tf; 
    // extern struct group* gf;
    // extern char input[];

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
