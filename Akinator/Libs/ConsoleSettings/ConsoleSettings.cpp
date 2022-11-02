#include <locale.h>
#include <windows.h>

#include "ConsoleSettings.h"

void init_console()
{
    setlocale(LC_ALL, "Russian");
}
