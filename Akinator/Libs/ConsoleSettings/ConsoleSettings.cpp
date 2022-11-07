#include <locale.h>
#include <assert.h>
#include <stdarg.h>

#include "ConsoleSettings.h"

void InitConsole()
{
    setlocale(LC_ALL, "Russian");
}

void ChangeColor(FILE* stream, size_t color)
{
    assert(stream != nullptr);

    fprintf(stream, "\033[%ldm", color);
}

void PrintInColor(FILE* stream, size_t color, const char* format, ...)
{
    assert(stream != nullptr);
    assert(format != nullptr);
    
    va_list args;
    va_start(args, format);

    fprintf(stream, "\033[%ldm", color);
    vprintf(format, args);
    fprintf(stream, "\033[%ldm", DEFAULT_COLOR);
} 