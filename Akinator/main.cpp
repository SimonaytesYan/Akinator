#include <stdio.h>

#include "Libs/ConsoleSettings/ConsoleSettings.h"
#include "Akinator.h"

int main()
{
    OpenHtmlLogFile("Akinator.html");
    init_console();
    
    Tree tree = {};
    TreeCtor(&tree);
    GetTreeFromFile(&tree, "Tree");



    SaveTreeInFile(&tree, "Tree");
    CloseHtmlLogFile();
}