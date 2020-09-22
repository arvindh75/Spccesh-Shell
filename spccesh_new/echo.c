#include "headers.h"
#include "echo.h"

void echo_f(char* suc) { 
    char* temp = "";
    char args[LS_SIZE][COM_LEN];
    for(int j=0; j < LS_SIZE; j++)
    {
        temp = strtok(NULL, " \t");
        if(temp == NULL)
        {
            for(int k = j; k < LS_SIZE; k++)
            {
                strcpy(args[k], "\0");
            }
            break;
        }
        int i = 0;
        for(i = 0; temp[i] != '\0'; i++)
        {
            args[j][i] = temp[i];
        }
        args[j][i] = '\0';
    }
    for(int j=0;j < LS_SIZE; j++) {
        if(args[j] != NULL) {
            printf("%s ",args[j]);
        }
    }
    printf("\n");
}
