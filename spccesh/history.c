#include "headers.h"
#include "history.h"
#include <stdio.h>

void add_his_f(char* home, char* inp, int dis) {
    FILE* f;
    char args[LS_SIZE][1000];
    char pathh[100];
    char* temp = "";
    int count = 0;
    char out[100];
    strcpy(pathh,home);
    strcat(pathh, "/spccesh_history.txt");
    f= fopen(pathh, "r");
    if(f==NULL) {
        f= fopen(pathh, "a");
        fclose(f);
        f= fopen(pathh, "r");
    }
    char his[21][20];
    while(fgets (out, sizeof(out), f)) {
            if(((int)out[1] >= 97 && (int)out[1] <=122) ||((int)out[1] >= 65 && (int)out[1] <=90) ) {
        //printf("COUNTING: %s\n", out);
        if(dis == 1) {
            printf("%s", out);
        }
            count++;
            }
    }
    fclose(f);
    if(dis == 1) {
        return;
    }
    //printf("PATH: %s\n", pathh);
    if(count < 5) {
        f= fopen(pathh, "a");
        if(f == NULL) {
            perror("History File");
            return;
        }
        else {
            //printf("--------------------\n");
            char* inpt = strtok(inp, " \t");
            //printf("ADDING: %s\n", inpt);
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
                //printf("ADDING: %s\n", args[j]);
                if(j== LS_SIZE -1) {
                    fprintf(f, "%s\n", args[j]);
                }
                fprintf(f, "%s ", args[j]);
            }
            //printf("--------------------\n");
        }
        fclose(f);
    }
    else {
        f= fopen(pathh, "r");
        for(int i=0;i < 5;) {
            fgets (out, 20, f);
            //printf("READING: %s\n", out);
            strcpy(his[i],out);
            //printf("WRITING: %s\n", his[i]);
            if(((int)out[1] >= 97 && (int)out[1] <=122) ||((int)out[1] >= 65 && (int)out[1] <=90) ) {
                i++;
            }
        }
        fclose(f);
        f= fopen(pathh, "w");
        if(f == NULL) {
            perror("History File");
            return;
        }

        //printf("--------------------\n");
        for(int i=1;i<5;) {
            //printf("ADDING: %s\n", his[i]);
            fprintf(f,"%s\n",his[i]);
            if(((int)his[i][1] >= 97 && (int)his[i][1] <=122) ||((int)his[i][1] >= 65 && (int)his[i][1] <=90) ) {
                i++;
            }
        }
        char* inpt = strtok(inp, " \t");
        //printf("ADDING: %s\n", inpt);
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
            //printf("ADDING: %s\n", args[j]);
            fprintf(f, "%s ", args[j]);
        }
        fprintf(f, "%s", "\n");
        //printf("--------------------\n");
        fclose(f);
    }
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
            if(number > 5 || number <=0) {
                printf("Argument must be less than 10 and positive");
                return;
            }
        }    
    }
    if(no_arg) {
        number = 5;
    }
    strcpy(pathh,home);
    strcat(pathh, "/spccesh_history.txt");
    //printf("PATH: %s\n", pathh);
    char out[100];
    int count=0;
    f= fopen(pathh, "r");
    while(fgets (out, sizeof(out), f)) {
        count++;
    }
    fclose(f);
    if(count < number) {
        number = count;
    }
    f= fopen(pathh, "r");
    char his[20][100];
    for(int i=0;i < number;i++) {
        //fscanf(f,"%s", out);
        fgets (out, 100, f);
        strcpy(his[i],out);
    }
    for(int i=number-1;i>=0;i--)
        printf("%s", his[i]);
    fclose(f);
}
