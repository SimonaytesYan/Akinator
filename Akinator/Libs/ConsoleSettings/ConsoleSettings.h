#ifndef __CONSOLE_SETTINGS_SYM__
#define __CONSOLE_SETTINGS_SYM__

#include <stdio.h>

enum COLORS
{
    DEFAULT_COLOR   = 0,
    RED             = 31,
    GREEN           = 32,
    YELLOW          = 33,
    BLUE            = 34,
    PURPLE          = 35,
    TURQUOISE       = 36,
};

void InitConsole();

void ChangeColor(FILE* stream, size_t color);

void PrintInColor(FILE* stream, size_t color, const char* format, ...);

#endif //__CONSOLE_SETTINGS_SYM__