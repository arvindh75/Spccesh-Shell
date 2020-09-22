#ifndef __EXECP_H
#define __EXECP_H

//void proc_end(int, char*, char*, char*, char*, char*);
void proc_end(int);
void contbg_f(char *);
void kjob_f(char *);
void ctrlz(int);
void ctrlc(int);
void bgfg_f(char *);
void overkill_f(char *);
void jobs_f(char *);
void str_replace_ep(char *, const char *, const char *);
void exec_proc_f(char*, char*, char*, char*, char*, char*, char*);

#endif
