#include "Logging.h"
#include "..\Errors.h"

#define LOGS_TO_FILE

FILE* logs_file = nullptr;

#include <stdarg.h>

int  OpenHtmlLogFile(const char* file_name)
{
    ReturnIfError(OpenLogFile(file_name));
    LogPrintf("<pre>\n");
    
    return 0;
}

int  CloseHtmlLogFile()
{
    return CloseLogFile();
}

int OpenLogFile(const char* file_name)
{
    logs_file = fopen(file_name, "w");
    if (logs_file == nullptr)
    {
        fprintf(stderr, "Error during open logs\n");
        return -1;
    }
    return 0;
}

int CloseLogFile()
{
    if (logs_file == nullptr)
        return -1;

    fclose(logs_file);
    return 0;
}

void ParseErrorCode(int error)
{
    for(int i = 0; i < ERRORS_NUMBER; i++)
    {
        if ((error & (1 << i)) != 0)
        {
            LogPrintf(ERROR_DESCRIPTION[i]);
        }
    }
    LogPrintf("\n");
}

int LogPrintf(const char *format, ...)
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