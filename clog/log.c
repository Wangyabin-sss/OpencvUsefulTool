#include "log.h"
#include <stdarg.h>
#include <time.h>


static char dbugbuf[DBUG_BUF_SIZE]={0};
static FILE *logfilefd=NULL;

void log_init(const char *filename)
{
#if (LOGMODEL==LOGFILE)
    logfilefd = fopen(filename, "w+");
    //数据全缓冲，填满后一次性写入文件
    setvbuf (logfilefd , dbugbuf, _IOFBF , DBUG_BUF_SIZE);
    fseek(logfilefd, 0, SEEK_END);
#endif
}

char *time_now(void)
{
    static char result[32];
    time_t tmval;
    time(&tmval);
    struct tm *tm;

    tm = localtime(&tmval);

    sprintf(result,
        "[%d/%02d/%02d %02d:%02d:%02d]",
        tm->tm_year + 1900,
        tm->tm_mon + 1,
        tm->tm_mday,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec);

    return result;
}


void log_2_cmd(int model, const char *fmt, ...)
{
    va_list valargs;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(valargs,fmt); //使args指向可变参数的第一个参数 
    char *timenow = time_now();

    switch (model)
    {
    case 0:
        printf("[info]");
#if LOG_WITHTIME
        printf("%s",timenow);
#endif
        vprintf(fmt,valargs);   
        break;
    case 1:
        printf("[error]");
#if LOG_WITHTIME
        printf("%s",timenow);
#endif
        vprintf(fmt,valargs); 
        break;
    case 2:
        printf("[warning]"); 
#if LOG_WITHTIME
        printf("%s",timenow);
#endif
        vprintf(fmt,valargs);  
        break;
    default:
        break;
    }
    va_end(valargs); //结束可变参数的获取

    fflush(stdout);  //QT Create需要使用
}


void log_2_file(int model, const char *fmt, ...)
{
    if(logfilefd == NULL)
        return;

    va_list valargs;  
    va_start(valargs,fmt);
    long filesize=0;
    char *timenow = time_now();

    switch (model)
    {
    case 0:
        fprintf(logfilefd, "[info]");
#if LOG_WITHTIME
        fprintf(logfilefd, "%s", timenow);
#endif
        vfprintf(logfilefd, fmt, valargs);
        break;
    case 1:
        fprintf(logfilefd, "[error]");
#if LOG_WITHTIME
        fprintf(logfilefd, "%s", timenow);
#endif
        vfprintf(logfilefd, fmt, valargs); 
        break;
    case 2:
        fprintf(logfilefd, "[warning]");
#if LOG_WITHTIME
        fprintf(logfilefd, "%s", timenow);
#endif
        vfprintf(logfilefd, fmt, valargs); 
        break;
    default:
        break;
    }
    va_end(valargs);
    filesize = ftell(logfilefd);
    if(filesize>LOGFILESIZE)
    {
    	printf("%ld\n",filesize);
        int ret = fseek(logfilefd, 0, SEEK_SET);
	printf("ret = %d\n",ret);
    }
}


void log_deinit(void)
{
    if(logfilefd!=NULL)
        fclose(logfilefd);
}


