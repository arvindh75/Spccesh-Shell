#include "prompt.h"
#include "headers.h"
#include "str_util.h"

void prompt_f() {
    extern cwd;
    extern tcwd;
    extern username;
    extern hostname;
    extern home;
    extern inp;
    extern input;
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
    }
    strcpy(tcwd,cwd);
    str_replace(cwd, home, "~");
    printf("<%s@%s:%s> : ", username, hostname, cwd);
    scanf("%[^\n]%*c", input);
    inp = strtok(input, " \t");
}
