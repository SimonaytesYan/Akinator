#include <locale.h>
#include <assert.h>
#include <stdarg.h>
#ifdef _WIN32
    #include <windows.h>
#endif

#include "ConsoleSettings.h"

void InitConsole()
{
    setlocale(LC_ALL, "Russian");
}

void ChangeColor(FILE* stream, short unsigned int color)
{
    assert(stream != nullptr);

    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    #else
        fprintf(stream, "\033[%dm", color);
    #endif
}

void PrintInColor(FILE* stream, short unsigned int color, const char* format, ...)
{
    assert(stream != nullptr);
    assert(format != nullptr);
    
    va_list args;
    va_start(args, format);

    ChangeColor(stream, color);
    vprintf(format, args);
    ChangeColor(stream, DEFAULT_COLOR);
} 