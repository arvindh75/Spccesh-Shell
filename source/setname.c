#include "setname.h"
#include "headers.h"

void setname_f() {
    extern home;
    extern result;
    extern username;
    extern hostname;
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
}
