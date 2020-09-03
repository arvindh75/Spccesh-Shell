#include "headers.h"
#include "history.h"

void add_his_f(char* home, char* inp) {
    FILE* f;
    char args[LS_SIZE][1000];
    char pathh[100];
    char* temp = "";
    strcpy(pathh,home);
    strcat(pathh, "/spccesh_history.txt");
    //printf("PATH: %s\n", pathh);
    f= fopen(pathh, "a");

    if(f == NULL) {
        perror("History File");
    }
    else {
        char* inpt = strtok(inp, " \t");
        fprintf(f, "%s ", inpt);
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
        for(int j=0; j < LS_SIZE; j++) {
            fprintf(f, "%s ", args[j]);
        }
    }
    fprintf(f, "%s", "\n");
    fclose(f);
}

void view_his_f(char* home) { 
    FILE* f;
    char pathh[100];
    char* temp = "";
    int number;
    int no_arg=1;
    char args[LS_SIZE][1000];
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
    for(int j=0; j < LS_SIZE; j++) {
        if(args[j][0] >=48 && args[j][0] <=57) {
            number = atoi(args[j]);
            no_arg=0;
            if(number > 10) {
                printf("Argument must be less than 10");
                return;
            }
        }    
    }
    if(no_arg) {
        number = 10;
    }
    strcpy(pathh,home);
    strcat(pathh, "/spccesh_history.txt");
    //printf("PATH: %s\n", pathh);
    f= fopen(pathh, "r");
    char out[100];
    char his[20][100];
    for(int i=0;i < number;i++) {
        //fscanf(f,"%s", out);
        fgets (out, 100, f);
        strcpy(his[i],out);
    }
    for(int i=number-1;i>=0;i--)
        printf("%s", his[i]);
}
