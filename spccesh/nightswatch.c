#include "headers.h"
#include "nightswatch.h"
clock_t start; 

int wait_f(int sec) {
    start= clock();
    char ch;
    struct termios t1, t2;
    int num_b;
    while(clock() - start - (sec*CLOCKS_PER_SEC) < 0) {
        tcgetattr(STDIN_FILENO, &t1);
        t2 = t1;
        t2.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t2);
        ioctl(STDIN_FILENO, FIONREAD, &num_b);
        tcsetattr(STDIN_FILENO, TCSANOW, &t1);
        if(num_b !=0) {
            if (getchar() == 'q')
            {
                return 0;
            }
        }
    }
    return 1;
}

void nw_f() { 
    FILE* f;
    char args[LS_SIZE][100];
    char* temp = "";
    char newpid[100];
    int num_arg=0;
    int number;
    int n_arg=0;
    int inter=0;
    int nb=0;
    char cpus[100];
    char newp[100];
    char timer_i[100];

    char keyb_i[100];
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
        if(args[j][0] == 45 && args[j][1] == 110) {
            n_arg = 1;
        }
        if(args[j][0] >=48 && args[j][0] <=57) {
            num_arg = 1;
            number = atoi(args[j]);
        }
        if(!strcmp(args[j], "interrupt")) {
            inter = 1;
        }
        else if(!strcmp(args[j], "newborn")) {
            nb = 1;
        }
    }
    if(!num_arg || !n_arg) {
        printf("Correct Arguments are not provided!\n");
    }
    else {
        if(inter) { 
            f = fopen("/proc/interrupts", "r");
            if(!f) {
                printf("Cannot open interrupts file!\n");
            }
            else {
                fgets(cpus, sizeof(cpus),f);
                fgets(timer_i, sizeof(timer_i),f);
                fgets(keyb_i, sizeof(keyb_i),f);
                printf("%s\n", cpus);
                printf("%s\n", keyb_i);
            }
            fclose(f);
            while(1)
            {
                if(wait_f(number) == 1) {
                    f = fopen("/proc/interrupts", "r");
                    if(!f) {
                        printf("Cannot open interrupts file!\n");
                    }
                    else {
                        fgets(cpus, sizeof(cpus),f);
                        fgets(timer_i, sizeof(timer_i),f);
                        fgets(keyb_i, sizeof(keyb_i),f);
                        printf("%s\n", keyb_i);
                    }
                    fclose(f);
                }
                else {
                    return;
                }
            }
        }
        else if(nb){
            f = fopen("/proc/loadavg", "r");
            if(!f) {
                printf("Cannot open loadavg file!\n");
            }
            else {
                fgets(newp, sizeof(newp),f);
                temp = strtok(newp, " ");
                while(temp != NULL) {
                    strcpy(newpid, temp);
                    //mprintf("%s", newpid);
                    temp = strtok(NULL, " ");
                }
                printf("%s", newpid);
            }
            fclose(f);
            while(1) {
                if(wait_f(number) == 1) {
                    f = fopen("/proc/loadavg", "r");
                    if(!f) {
                        printf("Cannot open loadavg file!\n");
                    }
                    else {
                        fgets(newp, sizeof(newp),f);
                        temp = strtok(newp, " ");
                        while(temp != NULL) {
                            //printf("%s\n", temp);
                            strcpy(newpid, temp);
                            temp = strtok(NULL, " ");
                        }
                        printf("%s", newpid);
                    }
                    fclose(f);
                }
                else {
                    return;
                }
            }
        }
    }
}
