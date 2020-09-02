#include "headers.h"
#include "ls.h"
//#include "str_util.h"

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

void ls_f(char* home, char* cwd, char* tcwd) { 
    //str function
    // extern int exit_loop;
    // extern char hostname[];
    // extern char username[];
    // extern char cwd[];
    // extern char tcwd[];
    // extern int result;
    // extern char* home; 
    // extern char* p;
    // extern char news[];
    // extern DIR* mydir;
    // extern DIR* ldir;
    // extern struct dirent *myfile;
    // extern struct stat mystat;
    // extern char buf[];
    // extern int maxlen;
    // extern int count;
    // extern int pass;
    // extern char args[][1000];
    // extern char* temp;
    // extern int a_ls;
    // extern int l_ls;
    // extern char* ls_dir;
    // extern int cnt_ls;
    // extern struct passwd* tf; 
    // extern struct group* gf;
    // extern char input[];
    int maxlen=0;
    int a_ls=0;
    int l_ls=0;
    int count = 0;
    int pass=0;
    char* temp = "";
    int cnt_ls = 0;
    DIR *mydir;
    DIR *ldir;
    struct dirent *myfile;
    struct stat mystat;
    char buf[BUF_SIZE];
    char* ls_dir;
    //struct passwd *tf; 
    //struct group *gf;
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
            if((args[j][0] >= 48 && args[j][0] <= 57) || (args[j][0] >= 97 && args[j][0] <= 122) || (args[j][0] >= 65 && args[j][0] <= 90) || (args[j][0] == 46) || (args[j][0] == 47) || (args[j][0] == 126)) {
                if(args[j][0] == '~') {
                    str_replace_ls(args[j],"~", home);
                }
                cnt_ls ++;
                ls_dir = args[j];
                mydir = opendir(ls_dir);
                if(mydir == NULL) {
                    printf("\nCannot open the Directory %s",ls_dir);
                    break;
                }
                ldir = opendir(ls_dir);
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

                        //tf = getpwuid(mystat.st_uid);
                        printf(" %s ", ((getpwuid(mystat.st_uid))->pw_name));

                        //gf = getgrgid(mystat.st_gid);
                        printf(" %s ", ((getgrgid(mystat.st_gid))->gr_name));

                        printf("%9zu ", mystat.st_size);
                        //printf("Blocks = %zu ", mystat.st_blocks);
                        //cprintf("%s ", ctime(&mystat.st_mtime));
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

                //tf = getpwuid(mystat.st_uid);
                //printf(" %s ", tf->pw_name);

                //gf = getgrgid(mystat.st_gid);
                //printf(" %s ", gf->gr_name);
                //tf = getpwuid(mystat.st_uid);
                printf(" %s ", ((getpwuid(mystat.st_uid))->pw_name));

                        //gf = getgrgid(mystat.st_gid);
                printf(" %s ", ((getgrgid(mystat.st_gid))->gr_name));

                printf("%9zu ", mystat.st_size);
                //printf("Blocks = %zu ", mystat.st_blocks);
                //cprintf("%s ", ctime(&mystat.st_mtime));
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
