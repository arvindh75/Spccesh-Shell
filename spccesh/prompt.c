#include "headers.h"
#include "prompt.h"

void red() {
  printf("\033[0;31m");
}

void blue() {
  printf("\033[0;34m");
}

void cyan() {
  printf("\033[1;36m");
}

void yellow() {
  printf("\033[0;33m");
}

void green() {
  printf("\033[1;32m");
}

void reset() {
  printf("\033[0m");
}

void str_replace(char* target, const char* needle, const char* replacement)
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

void prompt_f(char* home, char* username, char* hostname, char* cwd, char* tcwd) {
    char input[500];
    int len=0,c;
    char* temp = "";
    char args[LS_SIZE][1000];
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
    }
    strcpy(tcwd,cwd);
    str_replace(cwd, home, "~");
    green();
    printf("<");
    reset();
    cyan();
    printf("%s",username);
    reset();
    green();
    printf("@");
    reset();
    cyan();
    printf("%s", hostname);
    reset();
    green();
    printf(":");
    reset();
    cyan();
    printf("%s", cwd);
    reset();
    green();
    printf("> : ");
    reset();
}
