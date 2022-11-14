#ifndef __LOGGING_SYM__
#define __LOGGING_SYM__

#include <stdio.h>
#include <stdarg.h>

#include "StackErrors.h"

const char LOGS[] = "StackLogs.txt";
FILE* logs_file   = fopen(LOGS, "w");

size_t LogPrintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    #ifdef LOGS_TO_CONSOLE
        vprintf(format, args);
    #endif

    #ifdef LOGS_TO_FILE
        if (logs_file == nullptr)
            return ERROR_LOGS_OPEN;
        vfprintf(logs_file, format, args);
    #endif

    va_end(args);
    fflush(logs_file);

    return NO_ERROR;
}

#endif //__LOGGING_SYM__