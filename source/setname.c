#include "setname.h"
#include "headers.h"

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