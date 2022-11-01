#ifndef __LOGGING_SYM__
#define __LOGGING_SYM__

#include <stdio.h>

int  OpenLogFile(const char* file_name);

int  CloseLogFile();

void ParseErrorCode(int error);

int  OpenHtmlLogFile(const char* file_name);
int  CloseHtmlLogFile();

int  LogPrintf(const char *format, ...);

#define CHECK(cond, msg, return_code)                                                           \
    if (cond)                                                                                   \
    {                                                                                           \
        LogPrintf("In %s in %s(%d)\n", __PRETTY_FUNCTION__ , __FILE__, __LINE__);               \
        LogPrintf("%s\n", msg);                                                                 \
        return return_code;                                                                     \
    }

#define LogAndParseErr(cond, return_code)                                                      \
    if (cond)                                                                                  \
    {                                                                                          \
        LogPrintf("In %s in %s(%d)\n", __PRETTY_FUNCTION__ , __FILE__, __LINE__);              \
        ParseErrorCode(return_code);                                                           \
        return return_code;                                                                    \
    }

#define ReturnIfError(func)                 \
    {                                       \
        int error = func;                   \
        if (error != NO_ERROR)              \
            return error;                   \
    }

#endif //__LOGGING_SYM__
