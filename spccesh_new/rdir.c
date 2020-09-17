#include "headers.h"
#include "rdir.h"

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
    char* token = strtok(args, " \t><");
    while (token != NULL) {
        printf ("%s ", token);
        token = strtok (NULL, "><");
    }
}
