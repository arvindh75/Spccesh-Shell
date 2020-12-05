#include "headers.h"
#include "cd.h"

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

void cd_f(char* home) { 
    char fPath[2000];
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
    str_replace_cd(fPath,"\"","");
    if(chdir(fPath) < 0)
    {
        printf("cd: Not a valid path\n");
    }
}
