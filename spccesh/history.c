#include "headers.h"
#include "history.h"

void add_his_f(char* home, char* inp, int dis) {
    FILE* f;
    char args[LS_SIZE][100];
    char pathh[100];
    char* temp = "";
    int count = 0;
    char out[100];
    char last[100];
    char command[100];
    int number;
    int no_arg=1;
    int cnt_number;
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
            if(number > 10 || number <=0) {
                printf("Argument must be less than 10 and positive");
                return;
            }
        }    
    }
    if(no_arg) {
        number = 10;
    }
    cnt_number = 0;
    strcpy(pathh,home);
    strcat(pathh, "/spccesh_history.txt");
    f= fopen(pathh, "r");
    if(f==NULL) {
        f= fopen(pathh, "a");
        fclose(f);
        f= fopen(pathh, "r");
    }
    char his[11][100];
    while(fgets (out, sizeof(out), f)) {
        if(((int)out[1] >= 97 && (int)out[1] <=122) ||((int)out[1] >= 65 && (int)out[1] <=90) ) {
            strcpy(last,out);
            if(dis == 1) {
                if(cnt_number >= 10 - number)
                    printf("%s", out);
                cnt_number++;
            }
            count++;
        }
    }
    fclose(f);
    if(dis == 1) {
        return;
    }
    int length =0;
    char main_c[100];
    f= fopen(pathh, "a");
    if(f == NULL) {
        perror("History File");
        return;
    }
    else {
        char* inpt = strtok(inp, " \t");
        strcpy(main_c, inpt);
        length += sprintf(command + length, " %s ", inpt);
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
            if(j== LS_SIZE -1) {
                length += sprintf(command+length, "%s\n", args[j]);
            }
            length += sprintf(command+length, "%s ", args[j]);
        }
        command[strlen(command)-1] = '\0';
        //printf("LAST:\n");
        //for(int i=0;i<strlen(last);i++) {
        //    printf("%d th char: %c\n",i+1, last[i]);
        //}
        //printf("LAST+1: %ld %s\n",strlen(last+1), last+1);
        //printf("COMMAND:\n");
        // for(int i=0;i<strlen(command);i++) {
        //    printf("%d th char: %c\n",i+1, command[i]);
        //}
        if(!strcmp(last,command) || (!strcmp(last+1, command)) || (!strcmp(last,command+1))) {
            return;
        }
        if(count < 10) {
            fprintf(f, "%s ", main_c);
            for(int j=0; j < LS_SIZE; j++) {
                //printf("ADDING: %s\n", args[j]);
                if(j== LS_SIZE -1) {
                    fprintf(f, "%s\n", args[j]);
                }
                fprintf(f, "%s ", args[j]);
            }
            fclose(f);
        }
        else {
            f= fopen(pathh, "r");
            for(int i=0;i < 10;) {
                fgets (out, 99, f);
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
            for(int i=1;i<10;) {
                //printf("ADDING: %s\n", his[i]);
                fprintf(f,"%s\n",his[i]);
                if(((int)his[i][1] >= 97 && (int)his[i][1] <=122) ||((int)his[i][1] >= 65 && (int)his[i][1] <=90) ) {
                    i++;
                }
            }
            fprintf(f, "%s ", main_c);
            for(int j=0; j < LS_SIZE; j++) {
                //printf("ADDING: %s\n", args[j]);
                if(j== LS_SIZE -1) {
                    fprintf(f, "%s\n", args[j]);
                }
                fprintf(f, "%s ", args[j]);
            }            //printf("--------------------\n");
            fclose(f);
        }
    }
}
