#include "headers.h"
#include "prompt.h"
#include "str_util.h"

char* prompt_f(char* home, char* username, char* hostname, char* cwd, char* tcwd) {
    // extern char cwd[];
    // extern char tcwd[];
    // extern char username[];
    // extern char hostname[];
    // extern char* home;
    // extern char* inp;
    // extern char input[];
    char input[500];
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
    }
    strcpy(tcwd,cwd);
    str_replace(cwd, home, "~");
    printf("<%s@%s:%s> : ", username, hostname, cwd);
    scanf("%[^\n]%*c", input);
    char* inp = strtok(input, " \t");
    return inp;
}
