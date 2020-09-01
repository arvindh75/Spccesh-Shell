#include "headers.h"
#include "setname.h"

void setname_f(char* username, char* hostname) {
    // extern char* home;
    // extern int result;
    // extern char username[];
    // extern char hostname[];
    //home = getenv("HOME");
    int result = gethostname(hostname, HOST_NAME_MAX);
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