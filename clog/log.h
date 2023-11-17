#ifndef _LOG_H
#define _LOG_H

#define LOGCMD          0
#define LOGFILE         1
#define LOGNULL         2
#define LOGMODEL        LOGCMD
#define DBUG_BUF_SIZE   1024
#define LOGFILESIZE     3*1024*1024
#define LOG_WITHTIME    1

#include <stdio.h>

/*

*/
void log_init(const char *filename);
void log_2_cmd(int model, const char *fmt, ...);
void log_2_file(int model, const char *fmt, ...);
void log_deinit(void);


#if (LOGMODEL==LOGCMD)
#define Dbuginit()  do{log_init("");}while(0)
#define Dbuginfo(fmt , ...)   do{log_2_cmd(0, fmt , ##__VA_ARGS__);}while(0)
#define Dbugerror(fmt , ...)   do{log_2_cmd(1, fmt , ##__VA_ARGS__);}while(0)
#define Dbugwarning(fmt , ...)   do{log_2_cmd(2, fmt , ##__VA_ARGS__);}while(0)
#else
#define Dbuginit(file)  do{log_init(file);}while(0)
#define Dbuginfo(fmt , ...)   do{log_2_file(0, fmt , ##__VA_ARGS__);}while(0)
#define Dbugerror(fmt , ...)   do{log_2_file(1, fmt , ##__VA_ARGS__);}while(0)
#define Dbugwarning(fmt , ...)   do{log_2_file(2, fmt , ##__VA_ARGS__);}while(0)
#endif



#endif

