#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define LS_SIZE 10

void str_replace(char *target, const char *needle, const char *replacement)
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

int main() 
{
    int exit=0;
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    char cwd[PATH_MAX];
    char tcwd[PATH_MAX];
    int result;
    char* home; 
    char fPath[3000];
    char* p = "~";
    char new[1000];
    DIR *mydir;
    DIR *ldir;
    struct dirent *myfile;
    struct stat mystat;
    char buf[512];
    int maxlen=0;
    int count,pass;
    char args[LS_SIZE][1000];
    char* temp;
    int a_ls = 0;
    int l_ls = 0;
    char* ls_dir;
    int cnt_ls = 0;

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
    char input[100];
    while(exit ==  0) {
        if(getcwd(cwd, PATH_MAX) == NULL) {
            perror("getcwd()");
        }
        strcpy(tcwd,cwd);
        str_replace(cwd, home, "~");
        printf("<%s@%s:%s> : ", username, hostname, cwd);
        scanf("%[^\n]%*c", input);
        char* inp = strtok(input, " \t");
        if(inp != NULL) {

            if(!strcmp(inp, "exit")) {
                exit = 1;
            }

            else if(!strcmp(inp, "cd")) {
                if(!(p = strtok(NULL, " \t")))
                {
                    p = "~";
                }
                strcpy(new,p);
                if(p[0] == '~')
                {
                    strcpy(new, home);
                    strcat(new, p+1);
                }
                strcpy(fPath, new);
                if(chdir(fPath) < 0)
                {
                    printf("cd: Not a valid path\n");
                }
            }

            else if(!strcmp(inp, "pwd")) {
                printf("%s\n", tcwd);
            }

            else if(!strcmp(inp, "ls")) {
                a_ls=0;
                l_ls=0;
                count = 0;
                pass=0;
                temp = "";
                cnt_ls = 0;

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
                    if(args[j] != NULL) {
                        if(!strcmp(args[j], "-la")) {
                            a_ls = 1;
                            l_ls = 1;
                        }
                        if(!strcmp(args[j], "-a")) {
                            a_ls = 1;
                        }
                        if(!strcmp(args[j], "-l")) {
                            l_ls = 1;
                        }
                    }
                }

                for(int j=0;j < LS_SIZE; j++) {
                    if(args[j] != NULL) {
                        if((args[j][0] >= 97 && args[j][0] <= 122) || (args[j][0] >= 65 && args[j][0] <= 90) || (args[j][0] == 46) || (args[j][0] == 47) || (args[j][0] == 126)) {
                            if(args[j][0] == '~')
                                ls_dir = home;
                            else
                                ls_dir = args[j];
                            mydir = opendir(ls_dir);
                            if(mydir == NULL) {
                                printf("\nCannot open the Directory %s",ls_dir);
                                continue;
                            }
                            ldir = opendir(ls_dir);
                            cnt_ls ++;
                            printf("\n%s :\n", ls_dir);
                            maxlen = 0;
                            while((myfile = readdir(ldir)) != NULL) {
                                if(myfile->d_name[0] != '.') {
                                    if(strlen(myfile->d_name) > maxlen) {
                                        maxlen = strlen(myfile->d_name);
                                    } 
                                }
                            }
                            while((myfile = readdir(mydir)) != NULL)
                            {
                                if(a_ls == 1) {
                                    if(count == 0 && pass == 1) {
                                        printf("\n");
                                    }
                                    printf("%-*s",maxlen + 1,myfile->d_name);
                                    count=(count+1)%4;
                                    pass=1;
                                }
                                else { 
                                    if(myfile->d_name[0] != '.') {
                                        if(count == 0 && pass == 1) {
                                            printf("\n");
                                        }
                                        printf("%-*s",maxlen + 1,myfile->d_name);
                                        count=(count+1)%4;
                                        pass=1;
                                    }
                                }
                                //sprintf(buf, "%s/%s", tcwd, myfile->d_name);
                                //stat(buf, &mystat);
                                //printf("%zu",mystat.st_size);
                                //printf(" %s\n", myfile->d_name);
                            }
                            closedir(mydir);
                        }
                    }
                }
                if(cnt_ls == 0) {
                    mydir = opendir(tcwd);
                    ldir = opendir(tcwd);
                    maxlen = 0;
                    while((myfile = readdir(ldir)) != NULL) {
                        if(myfile->d_name[0] != '.') {
                            if(strlen(myfile->d_name) > maxlen) {
                                maxlen = strlen(myfile->d_name);
                            } 
                        }
                    }

                    while((myfile = readdir(mydir)) != NULL)
                    {
                        if(a_ls == 1) {
                            if(count == 0 && pass == 1) {
                                printf("\n");
                            }
                            printf("%-*s",maxlen + 1,myfile->d_name);
                            count=(count+1)%4;
                            pass=1;
                        }
                        else { 
                            if(myfile->d_name[0] != '.') {
                                if(count == 0 && pass == 1) {
                                    printf("\n");
                                }
                                printf("%-*s",maxlen + 1,myfile->d_name);
                                count=(count+1)%4;
                                pass=1;
                            }
                        }
                    }
                    closedir(mydir);
                }
                printf("\n");
            }

            else {
                printf("Command not found !\n");
            }
        }
        printf("\n");
    }
    return 0;
}
