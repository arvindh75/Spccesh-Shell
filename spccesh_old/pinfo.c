#include "headers.h"
#include "pinfo.h"

void str_replace_pinfo(char* target, const char* needle, const char* replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);
    while (1) {
        const char *p = strstr(tmp, needle);
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;
        tmp = p + needle_len;
    }
    strcpy(target, buffer);
}

void pinfo_f(char * inp, char* home) {
    pid_t id;
    int cnt=0;
    FILE* f;
    char* temp = "";
    char fpath[100];
    char tpath[100];
    char args[LS_SIZE][1000];
    char pstatus;
    long int mem; 
    char buff[1000];
    char pname[100];
    int x =1;
    int ret=-1;
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
    for(int j=0;j<LS_SIZE;j++) {
        if(args[j][0] != 0) {
            if(args[j][0] >= 48 && args[j][0] <= 57) {
                x=1;
                cnt ++;
                id = atoi(args[j]);
                strcpy(fpath,"/proc/");
                strcat(fpath, args[j]);
                strcpy(tpath, fpath);

                strcat(tpath, "/stat");
                f = fopen(tpath, "r");
                if(f == NULL) {
                    perror("\npinfo ");
                    continue;
                }
                printf("\nPid -- %d\n", id);
                while(x <= 23) {
                    fscanf(f,"%s",buff);
                    if(x == 3)
                        printf("Process status -- %s\n", buff);
                    if(x == 23)
                        printf("Memory -- %s\n", buff);
                    x++;
                }
                strcpy(tpath, fpath);
                strcat(tpath,"/exe");
                ret = readlink(tpath, buff, 1000);
                if(ret == -1) {
                    printf("Error finding the executable\n");
                }
                else {
                    buff[ret] = '\0';
                    str_replace_pinfo(buff,home,"~");
                    printf("Executable Path -- %s\n", buff);
                }
            }
            else {
                printf("Process %s not found. Displaying default.\n", args[j]);
            }
        }
    }
    if(cnt == 0) {
        id = getpid();
        char buf1[10];
        sprintf(buf1, "%d", id);
        strcpy(fpath,"/proc/");
        strcat(fpath,buf1);
        strcpy(tpath, fpath);

        strcat(tpath, "/stat");
        f = fopen(tpath, "r");
        if(f == NULL) {
            perror("pinfo ");
            return;
        }
        printf("\nPid -- %d\n", id);
        while(x <= 23) {
            fscanf(f,"%s",buff);
            if(x == 3)
                printf("Process Status -- %s\n", buff);
            if(x == 23)
                printf("Memory -- %s\n", buff);
            x++;
        } 
        strcpy(tpath, fpath);
        strcat(tpath,"/exe");
        ret = readlink(tpath, buff, 1000);
        if(ret == -1) {
            printf("Error finding the executable\n");
        }
        else {
            buff[ret] = '\0';
            str_replace_pinfo(buff,home,"~");
            printf("Executable Path -- %s\n", buff);
        }   
    }
}
