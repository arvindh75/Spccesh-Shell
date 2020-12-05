#include "headers.h"
#include "prompt.h"
#include "str_util.h"
#define LS_SIZE 25
#define BUF_SIZE 1024

void prompt_f() {
    // extern char cwd[];
    // extern char tcwd[];
    // extern char username[];
    // extern char hostname[];
    // extern char* home;
    // extern char* inp;
    // extern char input[];
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
    }
    strcpy(tcwd,cwd);
    str_replace(cwd, home, "~");
    printf("<%s@%s:%s> : ", username, hostname, cwd);
    scanf("%[^\n]%*c", input);
    inp = strtok(input, " \t");
}
