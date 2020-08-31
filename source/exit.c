#include "exit.h"
#include "headers.h"

void exit_f() {
    extern exit;
    exit = 1;  
}
