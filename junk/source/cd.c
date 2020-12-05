#include "headers.h"
#include "cd.h"
#define LS_SIZE 25
#define BUF_SIZE 1024

void cd_f() { 
    // extern char* home;
    // extern char* p;
    // extern char news[];
    char fPath[3000];
    if(!(p = strtok(NULL, " \t")))
    {
        p = "~";
    }
    strcpy(news,p);
    if(p[0] == '~')
    {
        strcpy(news, home);
        strcat(news, p+1);
    }
    strcpy(fPath, news);
    if(chdir(fPath) < 0)
    {
        printf("cd: Not a valid path\n");
    }
}
