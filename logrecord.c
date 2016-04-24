#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logrecord.h"

#define LR_DEBUG_FILE_	"lrdebug.log"
#define LR_MAX_STRING_LEN 		10240

/*Level类别*/
#define LR_NO_LOG_LEVEL			0
#define LR_DEBUG_LEVEL			1
#define LR_INFO_LEVEL			2
#define LR_WARNING_LEVEL		3
#define LR_ERROR_LEVEL			4;

int  LogLevel[5] = {LR_NO_LOG_LEVEL, LR_DEBUG_LEVEL, LR_INFO_LEVEL, LR_WARNING_LEVEL, LR_ERROR_LEVEL};

/*Level的名称*/
char LRLevelName[5][10] = {"NOLOG", "DEBUG", "INFO", "WARNING", "ERROR"};

static int LR_Error_GetCurTime(char* strTime)
{
	struct tm*		tmTime = NULL;
	size_t			timeLen = 0;
	time_t			tTime = 0;	
	
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	/*timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);*/
	timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);
	
	return timeLen;
}

static int LR_Error_OpenFile(int* pf)
{
	char	fileName[1024];
	
	memset(fileName, 0, sizeof(fileName));
#ifdef WIN32
	sprintf(fileName, "c:\\lrlog\\%s",LR_DEBUG_FILE_);
#else
	sprintf(fileName, "%s/log/%s", getenv("HOME"), LR_DEBUG_FILE_);
#endif
    
    *pf = open(fileName, O_WRONLY|O_CREAT|O_APPEND, 0666);
    if(*pf < 0)
    {
        return -1;
    }
	
	return 0;
}

static void LR_Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
    char str[LR_MAX_STRING_LEN];
    int	 strLen = 0;
    char tmpStr[64];
    int	 tmpStrLen = 0;
    int  pf = 0;
    
    /*初始化*/
    memset(str, 0, LR_MAX_STRING_LEN);
    memset(tmpStr, 0, 64);
    
    /*加入LOG时间*/
    tmpStrLen = LR_Error_GetCurTime(tmpStr);
    tmpStrLen = sprintf(str, "[%s] ", tmpStr);
    strLen = tmpStrLen;

    /*加入LOG等级*/
    tmpStrLen = sprintf(str+strLen, "[%s] ", LRLevelName[level]);
    strLen += tmpStrLen;
    
    /*加入LOG状态*/
    if (status != 0) 
    {
        tmpStrLen = sprintf(str+strLen, "[ERRNO is %d] ", status);
    }
    else
    {
    	tmpStrLen = sprintf(str+strLen, "[SUCCESS] ");
    }
    strLen += tmpStrLen;

    /*加入LOG信息*/
    tmpStrLen = vsprintf(str+strLen, fmt, args);
    strLen += tmpStrLen;

    /*加入LOG发生文件*/
    tmpStrLen = sprintf(str+strLen, " [%s]", file);
    strLen += tmpStrLen;

    /*加入LOG发生行数*/
    tmpStrLen = sprintf(str+strLen, " [%d]\n", line);
    strLen += tmpStrLen;
    
    /*打开LOG文件*/
    if(LR_Error_OpenFile(&pf))
	{
		return ;
	}
	
    /*写入LOG文件*/
    write(pf, str, strLen);
    /*LR_Log_Error_WriteFile(str);*/
    
    /*关闭文件*/
    close(pf);
    
    return ;
}

void LR_LOG(const char *file, int line, int level, int status, const char *fmt, ...)
{
    va_list args;
	
	/*判断是否需要写LOG*/
	/*if(level!=LR_DEBUG_LEVEL && level!=LR_INFO_LEVEL && level!=LR_WARNING_LEVEL && level!=LR_ERROR_LEVEL)*/
	if(level == LR_NO_LOG_LEVEL)
	{
		return ;
	}
	
	/*调用核心的写LOG函数*/
    va_start(args, fmt);
    LR_Error_Core(file, line, level, status, fmt, args);
    va_end(args);
    
    return ;
}
