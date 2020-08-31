#include "cd.h"
#include "headers.h"

void cd_f() { 
    // extern char* home;
    // extern char* p;
    // extern char new[];
    char fPath[3000];
    if(!(p = strtok(NULL, " \t")))
    {
        p = "~";
    }
    strcpy(new,p);
    if(p[0] == '~')
    {
        strcpy(new, home);
        strcat(new, p+1);
    }
    strcpy(fPath, new);
    if(chdir(fPath) < 0)
    {
        printf("cd: Not a valid path\n");
    }
}
