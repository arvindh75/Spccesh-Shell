#include "headers.h"
#include "setname.h"
#define LS_SIZE 25
#define BUF_SIZE 1024

void setname_f() {
    // extern char* home;
    // extern int result;
    // extern char username[];
    // extern char hostname[];
    home = getenv("HOME");
    result = gethostname(hostname, HOST_NAME_MAX);
    if (result)
    {
        perror("gethostname");
    }
    result = getlogin_r(username, LOGIN_NAME_MAX);
    if (result)
    {
        perror("getlogin_r");
    } 
}