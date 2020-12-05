#include "headers.h"
#include "cd.h"
#include <linux/limits.h>

void str_replace_cd(char* target, const char* needle, const char* replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);
    while (1) {
        const char *p = strstr(tmp, needle);
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;
        tmp = p + needle_len;
    }
    strcpy(target, buffer);
}

void cd_f(char* home, char* prwd, char* suc) { 
    char fPath[2000];
    char news[1000];
    char* p;
    char temp[PATH_MAX];
    //printf("PRWD: %s\n", prwd);
    if(!(p = strtok(NULL, " \t")))
    {
        p = "~";
    }
    strcpy(news,p);
    if(p[0] == '~'){
        strcpy(news, home);
        strcat(news, p+1);
    }
    if(p[0] == '-') {
        strcpy(news, prwd);
        strcat(news, p+1);
        printf("%s\n", prwd);
    }
    strcpy(fPath, news);
    str_replace_cd(fPath,"\"","");
    if(getcwd(temp, PATH_MAX) == NULL) {
        perror("getcwd()");
        strcpy(suc,"f");
    }
    //(printf("PREV: %s\n", temp);
    if(chdir(fPath) < 0)
    {
        printf("cd: Not a valid path\n");
        strcpy(suc,"f");
    }
    else {
        strcpy(prwd,temp);
        //printf("PREV2: %s\n", prwd);
    }
}
