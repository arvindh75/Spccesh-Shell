#include "headers.h"
#include "ls.h"

void str_replace_ls(char* target, const char* needle, const char* replacement)
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

void ls_f(char* home, char* cwd, char* tcwd, char* suc) { 
    int maxlen=0;
    int a_ls=0;
    int l_ls=0;
    int count = 0;
    int pass=0;
    char* temp = "";
    int cnt_ls = 0;
    DIR *mydir;
    DIR *ldir;
    DIR *sdir;
    struct dirent *myfile;
    struct stat mystat;
    char buf[BUF_SIZE];
    char* ls_dir;
    char args[LS_SIZE][COM_LEN];
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
        strcpy(suc,"f");
    }
    strcpy(tcwd,cwd);
    str_replace_ls(cwd, home, "~");


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
            if(args[j][0] == '-') {
                for(int i=0;i<strlen(args[j]);i++) {
                    if(args[j][i] == 'l') {
                        l_ls =1;
                    }
                    else if(args[j][i] == 'a') {
                        a_ls =1;
                    }
                    else if(args[j][i] != '-'){
                        printf("Flag not found!\n");
                        strcpy(suc,"f");
                        return;
                    }
                }
            }
        }
    }

    for(int j=0;j < LS_SIZE; j++) {
        if(args[j] != NULL) {
            if((args[j][0] >= 48 && args[j][0] <= 57) || (args[j][0] >= 97 && args[j][0] <= 122) || (args[j][0] >= 65 && args[j][0] <= 90) || (args[j][0] == 46) || (args[j][0] == 47) || (args[j][0] == 126)) {
                if(args[j][0] == '~') {
                    str_replace_ls(args[j],"~", home);
                }
                cnt_ls ++;
                ls_dir = args[j];
                mydir = opendir(ls_dir);
                if(mydir == NULL) {
                    printf("\nCannot open the Directory %s",ls_dir);
                    strcpy(suc,"f");
                    continue;
                }
                ldir = opendir(ls_dir);
                sdir = opendir(ls_dir);
                printf("\033[1;34m");
                printf("\n%s :\n", ls_dir);
                printf("\033[0m");
                maxlen = 0;
                while((myfile = readdir(ldir)) != NULL) {
                    if(myfile->d_name[0] != '.') {
                        if(strlen(myfile->d_name) > maxlen) {
                            maxlen = strlen(myfile->d_name);
                        } 
                    }
                }
                int size_b=0;
                while((myfile = readdir(sdir)) != NULL){
                    sprintf(buf, "%s/%s", tcwd, myfile->d_name);
                    stat(buf, &mystat);
                    if(a_ls == 1){
                        size_b += mystat.st_blocks;
                    }
                    else if(l_ls==1){
                        if(myfile->d_name[0] != '.') {
                            size_b += mystat.st_blocks;
                        }
                    }
                }
                if(a_ls ==1 || l_ls ==1)
                    printf("total %d\n", size_b);
                while((myfile = readdir(mydir)) != NULL)
                {
                    if(l_ls == 1) {
                        if(a_ls != 1) {
                            if (myfile->d_name[0] == '.') {
                                continue;
                            }
                        }
                        sprintf(buf, "%s/%s", ls_dir, myfile->d_name);
                        stat(buf, &mystat);

                        if((mystat.st_mode & S_IFMT) == S_IFBLK) {
                            printf("b");
                        }
                        else if((mystat.st_mode & S_IFMT) == S_IFCHR) {
                            printf("c");
                        }
                        else if((mystat.st_mode & S_IFMT) == S_IFDIR) {
                            printf("d");
                        }
                        else if((mystat.st_mode & S_IFMT) == S_IFIFO) {
                            printf("p");
                        }
                        else if((mystat.st_mode & S_IFMT) == S_IFLNK) {
                            printf("l");
                        }
                        else if((mystat.st_mode & S_IFMT) == S_IFSOCK) {
                            printf("s");
                        }
                        else {
                            printf("-");
                        }

                        printf( (mystat.st_mode & S_IRUSR) ? "r" : " -");
                        printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
                        printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
                        printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
                        printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
                        printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
                        printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
                        printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
                        printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");

                        printf(" %ld ", mystat.st_nlink);

                        printf(" %s ", ((getpwuid(mystat.st_uid))->pw_name));

                        printf(" %s ", ((getgrgid(mystat.st_gid))->gr_name));

                        printf("%9zu ", mystat.st_size);
                        char* c=ctime(&mystat.st_mtime);
                        for(int l=4;l<=15;l++)
                            printf("%c",c[l]);
                        printf(" ");
                        printf("%s\n", myfile->d_name);

                    }
                    else if(a_ls == 1) {
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
                printf("\n");
                closedir(mydir);
            }
        }
    }
    if(cnt_ls == 0) {
        mydir = opendir(tcwd);
        ldir = opendir(tcwd);
        sdir = opendir(tcwd);
        maxlen = 0;
        while((myfile = readdir(ldir)) != NULL) {
            if(myfile->d_name[0] != '.') {
                if(strlen(myfile->d_name) > maxlen) {
                    maxlen = strlen(myfile->d_name);
                } 
            }
        }
        int size_b=0;
        while((myfile = readdir(sdir)) != NULL){
            sprintf(buf, "%s/%s", tcwd, myfile->d_name);
            stat(buf, &mystat);
            if(a_ls == 1){
                size_b += mystat.st_blocks;
            }
            else if(l_ls==1) {
                if(myfile->d_name[0] != '.') {
                    size_b += mystat.st_blocks;
                }
            }
        }
        if(l_ls ==1)
            printf("total %d\n", size_b);
        while((myfile = readdir(mydir)) != NULL)
        {
            if(l_ls == 1) {
                if(a_ls != 1) {
                    if (myfile->d_name[0] == '.') {
                        continue;
                    }
                }
                sprintf(buf, "%s/%s", tcwd, myfile->d_name);
                stat(buf, &mystat);

                if((mystat.st_mode & S_IFMT) == S_IFBLK) {
                    printf("b");
                }
                else if((mystat.st_mode & S_IFMT) == S_IFCHR) {
                    printf("c");
                }
                else if((mystat.st_mode & S_IFMT) == S_IFDIR) {
                    printf("d");
                }
                else if((mystat.st_mode & S_IFMT) == S_IFIFO) {
                    printf("p");
                }
                else if((mystat.st_mode & S_IFMT) == S_IFLNK) {
                    printf("l");
                }
                else if((mystat.st_mode & S_IFMT) == S_IFSOCK) {
                    printf("s");
                }
                else {
                    printf("-");
                }

                printf( (mystat.st_mode & S_IRUSR) ? "r" : " -");
                printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
                printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
                printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
                printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
                printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
                printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
                printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
                printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");

                printf(" %ld ", mystat.st_nlink);

                printf(" %s ", ((getpwuid(mystat.st_uid))->pw_name));

                printf(" %s ", ((getgrgid(mystat.st_gid))->gr_name));

                printf("%9zu ", mystat.st_size);
                char* c=ctime(&mystat.st_mtime);
                for(int l=4;l<=15;l++)
                    printf("%c",c[l]);
                printf(" ");
                printf("%s\n", myfile->d_name);

            }
            else if(a_ls == 1) {
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
