#include <linux/limits.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
    int result;
    char* home; 
    home = getenv("HOME");
    printf("Home : %s\n", home);
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
        str_replace(cwd, home, "~");
        printf("<%s@%s:%s> : ", username, hostname, cwd);
        scanf("%s", input);
        if(!strcmp(input, "exit")) {
            exit = 1;
        }
        else {
            printf("Command not found !\n");
        }
    }
    return 0;
}
