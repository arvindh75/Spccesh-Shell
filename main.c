#include <linux/limits.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_SIZE 1000

void str_replace(char *target, const char *needle, const char *replacement)
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

int main() 
{
    int exit=0;
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    char cwd[PATH_MAX];
    char tcwd[PATH_MAX];
    int result;
    char* home; 
    char fPath[3000];
    home = getenv("HOME");
    result = gethostname(hostname, HOST_NAME_MAX);
    if (result)
    {
        perror("gethostname");
        return EXIT_FAILURE;
    }
    result = getlogin_r(username, LOGIN_NAME_MAX);
    if (result)
    {
        perror("getlogin_r");
        return EXIT_FAILURE;
    }
    char input[100];
    while(exit ==  0) {
        if(getcwd(cwd, PATH_MAX) == NULL) {
            perror("getcwd()");
        }
        strcpy(tcwd,cwd);
        str_replace(cwd, home, "~");
        printf("<%s@%s:%s> : ", username, hostname, cwd);
        scanf("%[^\n]%*c", input);
        char* inp = strtok(input, " \t");
        
        if(!strcmp(inp, "exit")) {
            exit = 1;
        }

        else if(!strcmp(inp, "cd")) {
            char* p = "~";
            char new[1000];
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

        else if(!strcmp(inp, "pwd")) {
            printf("%s\n", tcwd);
        }

        else {
            printf("Command not found !\n");
        }
    }
    return 0;
}
