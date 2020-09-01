#include "headers.h"
#include "prompt.h"
//#include "str_util.h"

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

// char* str_replace(const char* s, const char* oldW, const char* newW) 
// { 
//     char* result; 
//     int i, cnt = 0; 
//     int newWlen = strlen(newW); 
//     int oldWlen = strlen(oldW); 
  
//     // Counting the number of times old word 
//     // occur in the string 
//     for (i = 0; s[i] != '\0'; i++) { 
//         if (strstr(&s[i], oldW) == &s[i]) { 
//             cnt++; 
  
//             // Jumping to index after the old word. 
//             i += oldWlen - 1; 
//         } 
//     } 
  
//     // Making new string of enough length 
//     result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
//     i = 0; 
//     while (*s) { 
//         // compare the substring with the result 
//         if (strstr(s, oldW) == s) { 
//             strcpy(&result[i], newW); 
//             i += newWlen; 
//             s += oldWlen; 
//         } 
//         else
//             result[i++] = *s++; 
//     } 
  
//     result[i] = '\0'; 
//     return result; 
// } 

char* prompt_f(char* home, char* username, char* hostname, char* cwd, char* tcwd) {
    // extern char cwd[];
    // extern char tcwd[];
    // extern char username[];
    // extern char hostname[];
    // extern char* home;
    // extern char* inp;
    // extern char input[];
    char input[500];
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
    }
    strcpy(tcwd,cwd);
    // printf("Home : %s\n", home);
    // printf("Username : %s\n", username);
    // printf("Hostname : %s\n", hostname);
    // printf("CWD : %s\n", cwd);
    // printf("TCWD : %s\n", tcwd);
    str_replace(cwd, home, "~");

    // char* result; 
    // char *s = cwd;
    // char* oldW = home;
    // char* newW = "~"; 
    // int i, cnt = 0; 
    // int newWlen = strlen("~"); 
    // int oldWlen = sizeof(home)/sizeof(home[0]); 
  
    // // Counting the number of times old word 
    // // occur in the string 
    // for (i = 0; s[i] != '\0'; i++) { 
    //     if (strstr(&s[i], oldW) == &s[i]) { 
    //         cnt++; 
  
    //         // Jumping to index after the old word. 
    //         i += oldWlen - 1; 
    //     } 
    // } 
  
    // // Making new string of enough length 
    // result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    // i = 0; 
    // while (*s) { 
    //     // compare the substring with the result 
    //     if (strstr(s, oldW) == s) { 
    //         strcpy(&result[i], newW); 
    //         i += newWlen; 
    //         s += oldWlen; 
    //     } 
    //     else
    //         result[i++] = *s++; 
    // } 
  
    // result[i] = '\0'; 
    // cwd = result; 

    // char buffer[1024] = { 0 };
    // char *insert_point = &buffer[0];
    // const char *tmp = cwd;
    // size_t needle_len = strlen(home);
    // size_t repl_len = strlen("~");
    // while (1) {
    //     const char *p = strstr(tmp, home);
    //     if (p == NULL) {
    //         strcpy(insert_point, tmp);
    //         break;
    //     }
    //     memcpy(insert_point, tmp, p - tmp);
    //     insert_point += p - tmp;
    //     memcpy(insert_point, "~", repl_len);
    //     insert_point += repl_len;
    //     tmp = p + needle_len;
    // }
    // strcpy(cwd, buffer);


    printf("<%s@%s:%s> : ", username, hostname, cwd);
    scanf("%[^\n]%*c", input);
    char* inp = strtok(input, " \t");
    return inp;
}
