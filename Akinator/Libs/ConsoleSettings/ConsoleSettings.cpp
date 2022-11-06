#include <locale.h>
#include <assert.h>

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