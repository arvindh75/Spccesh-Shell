#ifndef __HEADER_H
#define __HEADER_H

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h> 
#include <sys/wait.h>	
#include <fcntl.h> 
#include <signal.h>
#include <unistd.h>


#define LS_SIZE 25
#define BUF_SIZE 1024

char args[LS_SIZE][1000];
 
int a_ls = 0;
int l_ls = 0;
char* temp;
int cnt_ls = 0;
char* home; 
//char fPath[3000];
char* p = "~";
char new[1000];
DIR *mydir;
DIR *ldir;
struct dirent *myfile;
struct stat mystat;
char buf[BUF_SIZE];
int maxlen=0;
char* ls_dir;
struct passwd *tf; 
struct group *gf;
int result;
char hostname[HOST_NAME_MAX];
char username[LOGIN_NAME_MAX];
char cwd[PATH_MAX];
char tcwd[PATH_MAX];
int exit_loop=0;
char input[100];
char* inp;

#endif