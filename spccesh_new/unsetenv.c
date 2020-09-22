#include "headers.h"
#include "unsetenv.h"

void unsetenv_f(char* suc) { 
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
    char var_name[100];
    int count=0;
    for(int j=0;j < LS_SIZE; j++) {
        if(args[j][0] != 0) {
            if(count == 0) {
                strcpy(var_name,args[j]);
            }
            count++;
        }
    }
    if(count > 1) {
        printf("Arguments provided are incorrect. Only 1 argument allowed!\n");
        strcpy(suc,"f");
        return;
    }
    if(unsetenv(var_name) == -1) {
        perror("unsetenv()");
        strcpy(suc,"f");
    }
    printf("\n");
}
