#include "cd.h"
#include "headers.h"

void cd_f() { 
    extern home;
    extern p;
    extern new;
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
