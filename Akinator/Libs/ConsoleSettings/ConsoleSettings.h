#ifndef __CONSOLE_SETTINGS_SYM__
#define __CONSOLE_SETTINGS_SYM__

#include <stdio.h>

#ifdef _WIN32
    enum COLORS
    {
        DEFAULT_COLOR   = 7,
        RED             = 12,
        GREEN           = 10,
        YELLOW          = 6,
        BLUE            = 9,
        PURPLE          = 13,
        TURQUOISE       = 11,
    };
#else
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
#endif

void InitConsole();

void ChangeColor(FILE* stream, size_t color);

void PrintInColor(FILE* stream, size_t color, const char* format, ...);

#endif //__CONSOLE_SETTINGS_SYM__