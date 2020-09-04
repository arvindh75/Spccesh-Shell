#include "headers.h"
#include "cd.h"

void cd_f(char* home) { 
    char fPath[3000];
    char news[1000];
    char* p ;
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
