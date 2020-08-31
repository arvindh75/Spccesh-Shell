#include "pwd.h"
#include "headers.h"

void pwd_f() {
    extern tcwd;
    printf("%s\n", tcwd);
}
