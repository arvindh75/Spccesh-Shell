#include "headers.h"
#include "rdir.h"
#include <unistd.h>

void str_replace_rdir(char* target, const char* needle, const char* replacement)
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

void rdir_f(char* args,char* home, char* cwd, char* tcwd) {
    char temp[100];
    char temp2[100];
    char left[100];
    char right[100];
    int stdin_save = dup(STDIN_FILENO);
    int stdout_save = dup(STDOUT_FILENO);
    strcpy(temp,args);
    strcpy(temp2,args);
    char* token1 = strtok(temp, " \t><");
    while (token1 != NULL) {
        printf ("%s ", token1);
        token1 = strtok (NULL, "><");
    }
    printf("\n\n");
    //char* token = strtok(args, " \t><");
    //while (token != NULL) {
    //    printf ("%s ", token);
    //    token = strtok (NULL, "><");
    //}
    char* ret = strstr(args, "> ");
    if(ret) {
        str_replace_rdir(temp2,ret, "");
        //printf("RET:%s\n", ret+2);
        //printf("TEM2:%s\n", temp2);
        strcpy(left,temp2);
        strcpy(right,ret+2);
    }
    char* ret2 = strstr(args, "< ");
    if(ret2)
        printf("RET: %s\n", ret2+2); 
}
