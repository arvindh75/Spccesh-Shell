#include "headers.h"
#include "pwd.h"
#define LS_SIZE 25
#define BUF_SIZE 1024

void pwd_f(char* tcwd, char* suc) {
    if(getcwd(tcwd, PATH_MAX) == NULL) {
        perror("getcwd()");
        strcpy(suc,"f");
    }
    printf("%s\n", tcwd);
}
