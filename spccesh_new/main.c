#include "headers.h"
#include "exit.h"
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "setname.h"
#include "prompt.h"
#include "exec_proc.h"
#include "pinfo.h"
#include "history.h"
#include "nightswatch.h"

#include "setenv.h"
#include "unsetenv.h"
#include "rdir.h"
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

void str_replace_main(char* target, const char* needle, const char* replacement)
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
void str_replace_main_1(char* target, const char* needle, const char* replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);
    const char *p = strstr(tmp, needle);
    if (p == NULL) {
        strcpy(insert_point, tmp);
        return;
    }
    memcpy(insert_point, tmp, p - tmp);
    insert_point += p - tmp;
    memcpy(insert_point, replacement, repl_len);
    insert_point += repl_len;
    tmp = p + needle_len;
    strcpy(insert_point, tmp);
    strcpy(target, buffer);
}

void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}

#define MAX_BUF_LEN 500

int main()
{
    int exit_loop = 0;
    char home_m[PATH_MAX];
    char hostname[HOST_NAME_MAX];  
    char username[LOGIN_NAME_MAX]; 
    char cwd[PATH_MAX];            
    char prwd[PATH_MAX];            
    char tcwd[PATH_MAX];           
    int len = 0;
    int c;
    int j;
    char input[100000];
    int num_args = 0;
    int num_args2 = 0;
    int exit_read = 0;
    int stdin_save = dup(0);
    int stdout_save = dup(1);
    int stdin_save1 = dup(0);
    int stdout_save1 = dup(1);
    char temp_rdir[100];
    char temp_rdir2[100];
    char args_rdir[100];
    char left[100];
    char right[100];
    char right2[100];
    int fd,fd2,fd3;
    int mulrd=0;
    int pip=0;
    int rd=0;
    int pipea[2];
    char inp_his[100];
    char pipcom[200][100];
    char* token;
    char* ppip;
    char* ret2;
    char* ret0, *ret, *rdi;
    char* inp;
    char suc[5];
    char prev_suc[5];
    char rdirinp[125], rdirout[125], rdirtemp[125];
    strcpy(suc,"s");
    strcpy(prev_suc, suc);
    int p, in, il, inputp, it, pq, leninp, andst, orst;
    char** args = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); 
    char** args2 = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); 
    char** argsrdir = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); 
    if (getcwd(home_m, PATH_MAX) == NULL){
        perror("getcwd()");
    }
    if (getcwd(prwd, PATH_MAX) == NULL){
        perror("getcwd()");
    }
    printf("\e[1;1H\e[2J"); // Clear Screen
    setname_f(username, hostname);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    while (exit_loop == 0)
    {
        setname_f(username, hostname);
        strcpy(suc,"s");
        //dup2(stdin_save, 0);
        //dup2(stdout_save, 1);
        //fflush(stdin);
        //fflush(stderr);
        //fflush(stdout);
        leninp=0;
        num_args=0;
        exit_read=0;
        prompt_f(home_m, username, hostname, cwd, tcwd, prev_suc);
        //fprintf(stderr,"\033[1;31m");
        while(exit_read == 0) {
            c = getchar();
            if(c == EOF) {
                exit_loop=1;
                return 0;
                //break;
            }
            if(c > 127 || c < 0)
                continue;
            //fprintf(stderr, "\n109:%c %d\n",c, leninp);
            if(c != '\n') {
                input[leninp] = c;
                leninp++;
            }
            else {
                input[leninp] = '\0';
                leninp++;
                exit_read=1;
            }
        }
        //ffprintf(stderr,"\033[0m");
        //fflush(stdin);
        //fflush(stderr);
        //fflush(stdout);
        str_replace_main(input,"&","&;");
        token = strtok(input, ";");
        num_args2=0;
        while(token!=NULL) {
            args2[num_args2] = token;
            num_args2++;
            token = strtok(NULL, ";");
        }
        int hj;
        for(hj=0;hj<num_args2;hj++) {
            strcpy(suc,"s");
            num_args=0;
            str_replace_main(args2[hj],"@","@;");
            str_replace_main(args2[hj],"$","$;");
            token = strtok(args2[hj], ";");
            while(token!=NULL) {
                args[num_args] = token;
                num_args++;
                token = strtok(NULL, ";");
            }
            andst=0;
            orst=0;
            //for(int jk=0; jk<num_args;jk++) {
            //    printf("--%s\n", args[jk]);
            //}
            for(j=0;j<num_args;j++) {
                //(printf("ARG-%s AND:%d OR:%d NUM_ARGS:%d NUM_ARGS2:%d J:%d\n", args[j], andst, orst, num_args,num_args2, j);
                if(andst == 1) {
                    if(strcmp(suc,"s")) {
                        if(j == num_args -1) {
                            //printf("HERE-173\n");
                            hj++;
                            break;
                        }
                        else {
                            //printf("HERE-177\n");
                            andst=0;
                            orst=0;
                            continue;
                        }
                    }
                }
                if(orst == 1) {
                    if(!strcmp(suc,"s")) {
                        if(j == num_args -1) {
                            //printf("HERE-187\n");
                            hj++;
                            break;
                        }
                        else {
                            //printf("HERE-191\n");
                            andst=0;
                            orst=0;
                            continue;
                        }
                    }
                }
                andst=0;
                orst=0;
                trim(args[j]);
                if(args[j][strlen(args[j])-1] == '@') {
                    //printf("HERE-203\n");
                    andst=1;
                }
                if(args[j][strlen(args[j])-1] == '$') {
                    //printf("HERE-207\n");
                    orst=1;
                }
                str_replace_main(args[j], "@", "");
                str_replace_main(args[j], "$", "");
                trim(args[j]);
                strcpy(suc,"s");
                rd=0;
                pip=0;
                fd=-1;
                fd2=-1;
                fd3=-1;
                mulrd=-1;
                if(args[j][0] != 0) {
                    strcpy(suc,"s");
                    dup2(stdin_save1, 0);
                    dup2(stdout_save1, 1);
                    for(p=0;p<200;p++) {
                        if(args[j][p] == '>' || args[j][p] == '<') {
                            rd=1;    
                        }
                        if(args[j][p] == '|') {
                            pip=1;
                        }
                    }
                    if(pip == 1) {
                        in = 0;
                        ppip=strtok(args[j],"|");
                        while (ppip) {
                            strcpy(pipcom[in], ppip);
                            ppip = strtok(NULL, "|");
                            in++;
                        }
                        for(il=0;il<in;il++) {
                            trim(pipcom[il]);
                        }
                        inputp = dup(0);
                        for(it=0; it<in; it++) {
                            strcpy(suc,"s");
                            rd=0;
                            fd=-1;
                            fd2=-1;
                            fd3=-1;
                            mulrd=-1;
                            //fprintf(stderr,"\033[1;31m");
                            //fprintf(stderr, "\n155");
                            for(pq=0;pq<100;pq++) {
                                //fprintf(stderr, "\n156: pq:% it:d%c\n\n",pipcom[it][pq]);
                                if(pipcom[it][pq] == '>' || pipcom[it][pq] == '<') {
                                    //fprintf(stderr, "\n157: Changing rd rd=%d\n\n", rd);
                                    rd=1;    
                                }
                            }
                            //fprintf(stderr,"\033[0m");
                            if(it == in-1) {
                                stdin_save = dup(0);
                                stdout_save = dup(1);
                                dup2(inputp, 0);
                                dup2(1,1);
                                //input,1;
                                //echar* inp = strtok(pipcom[it], " \t");
                                //exec_main(pipcom[it], inputp, 1);
                                //exec_proc_f(inp, home_m, username, hostname, cwd, tcwd);
                                strcpy(args[j], pipcom[it]);
                                //char* inp = strtok(pipcom[it], " \t");
                                //exec_main(pipcom[it], inputp, 0);
                                //exec_proc_f(inp, home_m, username, hostname, cwd, tcwd);
                                if(rd == 1) {
                                    int cn=0;
                                    int append=-1;
                                    strcpy(inp_his,args[j]);
                                    strcpy(rdirinp, "\0");
                                    strcpy(rdirout, "\0");
                                    //printf("s:%s:e\n", args[j]);
                                    rdi = strtok(args[j], " \t");
                                    while(rdi!=NULL) {
                                        //printf("- %s\n", rdi);
                                        argsrdir[cn] = rdi;
                                        cn++;
                                        rdi = strtok(NULL, " \t");
                                    }
                                    for(int yl=0;yl<cn;yl++) {
                                        //printf("-- %s\n", argsrdir[yl]);
                                        if(argsrdir[yl][0] == '<') {
                                            strcpy(rdirinp, argsrdir[yl+1]);
                                            strcpy(argsrdir[yl],"\0");
                                            strcpy(argsrdir[yl+1],"\0");
                                        }
                                        else if(argsrdir[yl][0] == '>' && argsrdir[yl][1] == '>') {
                                            strcpy(rdirout, argsrdir[yl+1]);
                                            append=1;
                                            strcpy(argsrdir[yl],"\0");
                                            strcpy(argsrdir[yl+1],"\0");
                                        }
                                        else if(argsrdir[yl][0] == '>') {
                                            strcpy(rdirout, argsrdir[yl+1]);
                                            append=0;
                                            strcpy(argsrdir[yl],"\0");
                                            strcpy(argsrdir[yl+1],"\0");
                                        }
                                    }
                                    strcpy(rdirtemp,"");
                                    for(int yl=0;yl<cn;yl++) {
                                        if(strcmp(argsrdir[yl], "\0")) {
                                            //rprintf("^^%s\n", argsrdir[yl]);
                                            strcat(rdirtemp, argsrdir[yl]);
                                            strcat(rdirtemp, " ");
                                        }
                                    }
                                    trim(rdirtemp);
                                    strcpy(args[j], rdirtemp);
                                    //printf("\nINPUT:%s\n", rdirinp);
                                    //printf("OUTPUT:%s\n", rdirout);
                                    //printf("COMMAND:%s\n", args[j]);
                                    //printf("APPEND:%d\n\n", append);
                                    if(strcmp(rdirinp, "\0")) {
                                        fd = open(rdirinp, O_RDONLY);
                                        if(fd < 0) {
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            printf("Cannot open file %s\n", rdirinp);
                                            break;
                                        }
                                        if(dup2(fd, STDIN_FILENO) == -1) {
                                            perror("Duplicating file descriptor.");
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            break;
                                        }
                                    }
                                    if(strcmp(rdirout, "\0")) {
                                        if(append == 0)
                                            fd2 = open(rdirout, O_WRONLY | O_CREAT, 0644);
                                        else if (append == 1)
                                            fd2 = open(rdirout, O_WRONLY | O_CREAT | O_APPEND, 0644);
                                        if(fd2 < 0) {
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            printf("Cannot open file %s\n", rdirout);
                                            break;
                                        }
                                        if(dup2(fd2, STDOUT_FILENO) == -1) {
                                            perror("Duplicating file descriptor.");
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            break;
                                        }
                                    }
                                    rd=0;
                                    add_his_f(home_m, inp_his, 0, suc);
                                }
                                if(rd == 0) {
                                    //fprintf(stderr,"\033[1;31m");
                                    //fprintf(stderr, "\n278:%s\n\n",args[j]);
                                    //fprintf(stderr,"\033[0m");
                                    strcpy(inp_his,args[j]);
                                    inp="";
                                    inp = strtok(args[j], " \t");
                                    if (inp != NULL)
                                    {
                                        if ((!strcmp(inp, "exit")) || (!strcmp(inp, "quit")))
                                        {
                                            exit_loop = exit_f();
                                        }

                                        else if (!strcmp(inp, "cd"))
                                        {
                                            cd_f(home_m, prwd, suc);
                                        }

                                        else if (!strcmp(inp, "pwd"))
                                        {
                                            pwd_f(tcwd, suc);
                                        }

                                        else if (!strcmp(inp, "echo"))
                                        {
                                            echo_f(suc);
                                        }

                                        else if (!strcmp(inp, "ls"))
                                        {
                                            ls_f(home_m, cwd, tcwd, suc);
                                        }

                                        else if (!strcmp(inp, "pinfo"))
                                        {
                                            pinfo_f(inp, home_m, suc);
                                        }

                                        else if (!strcmp(inp, "history"))
                                        {
                                            add_his_f(home_m, inp_his, 1, suc);
                                        }

                                        else if (!strcmp(inp, "nightswatch"))
                                        {
                                            nw_f(suc);
                                        }

                                        else if (!strcmp(inp, "setenv"))
                                        {
                                            setenv_f(suc);
                                        }

                                        else if (!strcmp(inp, "unsetenv"))
                                        {
                                            unsetenv_f(suc);
                                        }

                                        else if (!strcmp(inp, "jobs"))
                                        {
                                            jobs_f(suc);
                                        }

                                        else if (!strcmp(inp, "overkill"))
                                        {
                                            overkill_f(suc);
                                        }

                                        else if (!strcmp(inp, "kjob"))
                                        {
                                            kjob_f(suc);
                                        }

                                        else if (!strcmp(inp, "bg"))
                                        {
                                            contbg_f(suc);
                                        }

                                        else if (!strcmp(inp, "fg"))
                                        {
                                            bgfg_f(suc);
                                        }

                                        else
                                        {
                                            exec_proc_f(inp, home_m, username, hostname, cwd, tcwd, suc);
                                        }
                                        if(fd==-1 && fd2==-1 && fd3==-1)
                                            add_his_f(home_m, inp_his, 0, suc);
                                    }
                                    if(fd != -1)
                                        close(fd);
                                    if(fd2 != -1)
                                        close(fd2);
                                    if(fd3 != -1)
                                        close(fd3);
                                }
                                //close(pipea[1]);
                                dup2(stdin_save, 0);
                                dup2(stdout_save, 1);
                                close(stdin_save);
                                close(stdout_save);
                                close(pipea[0]);
                                close(pipea[1]);
                                fflush(stdin);
                                fflush(stdout);
                            }
                            else {
                                pipe(pipea);
                                stdin_save = dup(0);
                                stdout_save = dup(1);
                                dup2(inputp, 0);
                                dup2(pipea[1],1);
                                //input,pipea[1];
                                strcpy(args[j], pipcom[it]);
                                //char* inp = strtok(pipcom[it], " \t");
                                //exec_main(pipcom[it], inputp, 0);
                                //exec_proc_f(inp, home_m, username, hostname, cwd, tcwd);
                                //fprintf(stderr,"\033[1;31m");
                                //fprintf(stderr, "\n381:%s rd=%d\n\n",args[j], rd);
                                //fprintf(stderr,"\033[0m");
                                if(rd == 1) {
                                    int cn=0;
                                    int append=-1;
                                    strcpy(inp_his,args[j]);
                                    strcpy(rdirinp, "\0");
                                    strcpy(rdirout, "\0");
                                    //printf("s:%s:e\n", args[j]);
                                    rdi = strtok(args[j], " \t");
                                    while(rdi!=NULL) {
                                        //printf("- %s\n", rdi);
                                        argsrdir[cn] = rdi;
                                        cn++;
                                        rdi = strtok(NULL, " \t");
                                    }
                                    for(int yl=0;yl<cn;yl++) {
                                        //printf("-- %s\n", argsrdir[yl]);
                                        if(argsrdir[yl][0] == '<') {
                                            strcpy(rdirinp, argsrdir[yl+1]);
                                            strcpy(argsrdir[yl],"\0");
                                            strcpy(argsrdir[yl+1],"\0");
                                        }
                                        else if(argsrdir[yl][0] == '>' && argsrdir[yl][1] == '>') {
                                            strcpy(rdirout, argsrdir[yl+1]);
                                            append=1;
                                            strcpy(argsrdir[yl],"\0");
                                            strcpy(argsrdir[yl+1],"\0");
                                        }
                                        else if(argsrdir[yl][0] == '>') {
                                            strcpy(rdirout, argsrdir[yl+1]);
                                            append=0;
                                            strcpy(argsrdir[yl],"\0");
                                            strcpy(argsrdir[yl+1],"\0");
                                        }
                                    }
                                    strcpy(rdirtemp,"");
                                    for(int yl=0;yl<cn;yl++) {
                                        if(strcmp(argsrdir[yl], "\0")) {
                                            //rprintf("^^%s\n", argsrdir[yl]);
                                            strcat(rdirtemp, argsrdir[yl]);
                                            strcat(rdirtemp, " ");
                                        }
                                    }
                                    trim(rdirtemp);
                                    strcpy(args[j], rdirtemp);
                                    //printf("\nINPUT:%s\n", rdirinp);
                                    //printf("OUTPUT:%s\n", rdirout);
                                    //printf("COMMAND:%s\n", args[j]);
                                    //printf("APPEND:%d\n\n", append);
                                    if(strcmp(rdirinp, "\0")) {
                                        fd = open(rdirinp, O_RDONLY);
                                        if(fd < 0) {
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            printf("Cannot open file %s\n", rdirinp);
                                            break;
                                        }
                                        if(dup2(fd, STDIN_FILENO) == -1) {
                                            perror("Duplicating file descriptor.");
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            break;
                                        }
                                    }
                                    if(strcmp(rdirout, "\0")) {
                                        if(append == 0)
                                            fd2 = open(rdirout, O_WRONLY | O_CREAT, 0644);
                                        else if (append == 1)
                                            fd2 = open(rdirout, O_WRONLY | O_CREAT | O_APPEND, 0644);
                                        if(fd2 < 0) {
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            printf("Cannot open file %s\n", rdirout);
                                            break;
                                        }
                                        if(dup2(fd2, STDOUT_FILENO) == -1) {
                                            perror("Duplicating file descriptor.");
                                            strcpy(suc,"f");
                                            fflush(stdout);
                                            if(fd != -1)
                                                close(fd);
                                            if(fd2 != -1)
                                                close(fd2);
                                            if(fd3 != -1)
                                                close(fd3);
                                            dup2(stdin_save, 0);
                                            dup2(stdout_save, 1);
                                            close(stdin_save);
                                            close(stdout_save);
                                            break;
                                        }
                                    }
                                    rd=0;
                                    add_his_f(home_m, inp_his, 0, suc);
                                }
                                if(rd == 0) {
                                    //fprintf(stderr,"\033[1;31m");
                                    //fprintf(stderr, "\n487:%s\n\n",args[j]);
                                    //fprintf(stderr,"\033[0m");
                                    strcpy(inp_his,args[j]);
                                    inp="";
                                    inp = strtok(args[j], " \t");
                                    if (inp != NULL)
                                    {
                                        if ((!strcmp(inp, "exit")) || (!strcmp(inp, "quit")))
                                        {
                                            exit_loop = exit_f();
                                        }

                                        else if (!strcmp(inp, "cd"))
                                        {
                                            cd_f(home_m, prwd,suc);
                                        }

                                        else if (!strcmp(inp, "pwd"))
                                        {
                                            pwd_f(tcwd, suc);
                                        }

                                        else if (!strcmp(inp, "echo"))
                                        {
                                            echo_f(suc);
                                        }

                                        else if (!strcmp(inp, "ls"))
                                        {
                                            ls_f(home_m, cwd, tcwd, suc);
                                        }

                                        else if (!strcmp(inp, "pinfo"))
                                        {
                                            pinfo_f(inp, home_m, suc);
                                        }

                                        else if (!strcmp(inp, "history"))
                                        {
                                            add_his_f(home_m, inp_his, 1, suc);
                                        }

                                        else if (!strcmp(inp, "nightswatch"))
                                        {
                                            nw_f(suc);
                                        }

                                        else if (!strcmp(inp, "setenv"))
                                        {
                                            setenv_f(suc);
                                        }

                                        else if (!strcmp(inp, "unsetenv"))
                                        {
                                            unsetenv_f(suc);
                                        }

                                        else if (!strcmp(inp, "jobs"))
                                        {
                                            jobs_f(suc);
                                        }

                                        else if (!strcmp(inp, "overkill"))
                                        {
                                            overkill_f(suc);
                                        }

                                        else if (!strcmp(inp, "kjob"))
                                        {
                                            kjob_f(suc);
                                        }

                                        else if (!strcmp(inp, "bg"))
                                        {
                                            contbg_f(suc);
                                        }

                                        else if (!strcmp(inp, "fg"))
                                        {
                                            bgfg_f(suc);
                                        }

                                        else
                                        {
                                            exec_proc_f(inp, home_m, username, hostname, cwd, tcwd, suc);
                                        }
                                        if(fd==-1 && fd2==-1 && fd3==-1)
                                            add_his_f(home_m, inp_his, 0, suc);
                                    }
                                    if(fd != -1)
                                        close(fd);
                                    if(fd2 != -1)
                                        close(fd2);
                                    if(fd3 != -1)
                                        close(fd3);
                                }
                                dup2(stdin_save, 0);
                                dup2(stdout_save, 1);
                                close(stdin_save);
                                close(stdout_save);
                                close(pipea[1]);
                                inputp = pipea[0];
                            }
                        }
                    }
                    else {
                        stdin_save = dup(0);
                        stdout_save = dup(1);
                        //fprintf(stderr,"\033[1;31m");
                        //fprintf(stderr, "\nA:%s\n\n",args[j]);
                        //fprintf(stderr, "\nPIP=0:\n\n");
                        //fprintf(stderr,"\033[0m");
                        if(rd == 1) {
                            int cn=0;
                            int append=-1;
                            strcpy(inp_his,args[j]);
                            strcpy(rdirinp, "\0");
                            strcpy(rdirout, "\0");
                            //printf("s:%s:e\n", args[j]);
                            rdi = strtok(args[j], " \t");
                            while(rdi!=NULL) {
                                //printf("- %s\n", rdi);
                                argsrdir[cn] = rdi;
                                cn++;
                                rdi = strtok(NULL, " \t");
                            }
                            for(int yl=0;yl<cn;yl++) {
                                //printf("-- %s\n", argsrdir[yl]);
                                if(argsrdir[yl][0] == '<') {
                                    strcpy(rdirinp, argsrdir[yl+1]);
                                    strcpy(argsrdir[yl],"\0");
                                    strcpy(argsrdir[yl+1],"\0");
                                }
                                else if(argsrdir[yl][0] == '>' && argsrdir[yl][1] == '>') {
                                    strcpy(rdirout, argsrdir[yl+1]);
                                    append=1;
                                    strcpy(argsrdir[yl],"\0");
                                    strcpy(argsrdir[yl+1],"\0");
                                }
                                else if(argsrdir[yl][0] == '>') {
                                    strcpy(rdirout, argsrdir[yl+1]);
                                    append=0;
                                    strcpy(argsrdir[yl],"\0");
                                    strcpy(argsrdir[yl+1],"\0");
                                }
                            }
                            strcpy(rdirtemp,"");
                            for(int yl=0;yl<cn;yl++) {
                                if(strcmp(argsrdir[yl], "\0")) {
                                    //rprintf("^^%s\n", argsrdir[yl]);
                                    strcat(rdirtemp, argsrdir[yl]);
                                    strcat(rdirtemp, " ");
                                }
                            }
                            trim(rdirtemp);
                            strcpy(args[j], rdirtemp);
                            //printf("\nINPUT:%s\n", rdirinp);
                            //printf("OUTPUT:%s\n", rdirout);
                            //printf("COMMAND:%s\n", args[j]);
                            //printf("APPEND:%d\n\n", append);
                            if(strcmp(rdirinp, "\0")) {
                                fd = open(rdirinp, O_RDONLY);
                                if(fd < 0) {
                                    strcpy(suc,"f");
                                    fflush(stdout);
                                    if(fd != -1)
                                        close(fd);
                                    if(fd2 != -1)
                                        close(fd2);
                                    if(fd3 != -1)
                                        close(fd3);
                                    dup2(stdin_save, 0);
                                    dup2(stdout_save, 1);
                                    close(stdin_save);
                                    close(stdout_save);
                                    printf("Cannot open file %s\n", rdirinp);
                                    break;
                                }
                                if(dup2(fd, STDIN_FILENO) == -1) {
                                    perror("Duplicating file descriptor.");
                                    strcpy(suc,"f");
                                    fflush(stdout);
                                    if(fd != -1)
                                        close(fd);
                                    if(fd2 != -1)
                                        close(fd2);
                                    if(fd3 != -1)
                                        close(fd3);
                                    dup2(stdin_save, 0);
                                    dup2(stdout_save, 1);
                                    close(stdin_save);
                                    close(stdout_save);
                                    break;
                                }
                            }
                            if(strcmp(rdirout, "\0")) {
                                if(append == 0)
                                    fd2 = open(rdirout, O_WRONLY | O_CREAT, 0644);
                                else if (append == 1)
                                    fd2 = open(rdirout, O_WRONLY | O_CREAT | O_APPEND, 0644);
                                if(fd2 < 0) {
                                    strcpy(suc,"f");
                                    fflush(stdout);
                                    if(fd != -1)
                                        close(fd);
                                    if(fd2 != -1)
                                        close(fd2);
                                    if(fd3 != -1)
                                        close(fd3);
                                    dup2(stdin_save, 0);
                                    dup2(stdout_save, 1);
                                    close(stdin_save);
                                    close(stdout_save);
                                    printf("Cannot open file %s\n", rdirout);
                                    break;
                                }
                                if(dup2(fd2, STDOUT_FILENO) == -1) {
                                    perror("Duplicating file descriptor.");
                                    strcpy(suc,"f");
                                    fflush(stdout);
                                    if(fd != -1)
                                        close(fd);
                                    if(fd2 != -1)
                                        close(fd2);
                                    if(fd3 != -1)
                                        close(fd3);
                                    dup2(stdin_save, 0);
                                    dup2(stdout_save, 1);
                                    close(stdin_save);
                                    close(stdout_save);
                                    break;
                                }
                            }
                            rd=0;
                            add_his_f(home_m, inp_his, 0, suc);
                        }
                        if(rd == 0) {
                            //fprintf(stderr,"\033[1;31m");
                            //fprintf(stderr, "\nA:%s\n\n",args[j]);
                            //fprintf(stderr, "\nPIP=0:\n\n");
                            //fprintf(stderr,"\033[0m");
                            strcpy(inp_his,args[j]);
                            inp="";
                            inp = strtok(args[j], " \t");
                            if (inp != NULL)
                            {
                                if ((!strcmp(inp, "exit")) || (!strcmp(inp, "quit")))
                                {
                                    exit_loop = exit_f();
                                }

                                else if (!strcmp(inp, "cd"))
                                {
                                    cd_f(home_m, prwd, suc);
                                }

                                else if (!strcmp(inp, "pwd"))
                                {
                                    pwd_f(tcwd, suc);
                                }

                                else if (!strcmp(inp, "echo"))
                                {
                                    echo_f(suc);
                                }

                                else if (!strcmp(inp, "ls"))
                                {
                                    ls_f(home_m, cwd, tcwd, suc);
                                }

                                else if (!strcmp(inp, "pinfo"))
                                {
                                    pinfo_f(inp, home_m, suc);
                                }

                                else if (!strcmp(inp, "history"))
                                {
                                    add_his_f(home_m, inp_his, 1, suc);
                                }

                                else if (!strcmp(inp, "nightswatch"))
                                {
                                    nw_f(suc);
                                }

                                else if (!strcmp(inp, "setenv"))
                                {
                                    setenv_f(suc);
                                }

                                else if (!strcmp(inp, "unsetenv"))
                                {
                                    unsetenv_f(suc);
                                }

                                else if (!strcmp(inp, "jobs"))
                                {
                                    jobs_f(suc);
                                }

                                else if (!strcmp(inp, "overkill"))
                                {
                                    overkill_f(suc);
                                }

                                else if (!strcmp(inp, "kjob"))
                                {
                                    kjob_f(suc);
                                }

                                else if (!strcmp(inp, "bg"))
                                {
                                    contbg_f(suc);
                                }

                                else if (!strcmp(inp, "fg"))
                                {
                                    bgfg_f(suc);
                                }

                                else
                                {
                                    exec_proc_f(inp, home_m, username, hostname, cwd, tcwd, suc);
                                }
                                if(fd==-1 && fd2==-1 && fd3==-1)
                                    add_his_f(home_m, inp_his, 0, suc);
                            }
                            //dup2(stdin_save, STDIN_FILENO);
                            //dup2(stdout_save, STDOUT_FILENO);
                            fflush(stdout);
                            if(fd != -1)
                                close(fd);
                            if(fd2 != -1)
                                close(fd2);
                            if(fd3 != -1)
                                close(fd3);
                            dup2(stdin_save, 0);
                            dup2(stdout_save, 1);
                            close(stdin_save);
                            close(stdout_save);
                            //printf("HERE________\n");
                        }
                    }
                }
                //printf("HERE2________\n");
            }
        }
        printf("\n");
        strcpy(prev_suc,suc);
    }
    return 0;
}
