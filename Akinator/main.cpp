#include <stdio.h>
#include <locale.h>
#include <time.h>

#include "Libs/ConsoleSettings/ConsoleSettings.h"
#include "Akinator.h"

int main()
{
    OpenHtmlLogFile("Akinator.html");
    srand((unsigned int)time(0));

    #ifdef _WIN32
        InitConsole();
    #endif
    
    Tree tree = {};
    TreeCtor(&tree);
    GetTreeFromFile(&tree, DEFAULT_TREE_NAME);

    RunAkinator(&tree);
    
    SaveTreeInFile(&tree, DEFAULT_TREE_NAME);
    TreeDtor(&tree);

    CloseHtmlLogFile();
}