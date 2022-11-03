#include <stdio.h>
#include <locale.h>

#include "Libs/ConsoleSettings/ConsoleSettings.h"
#include "Akinator.h"

int main()
{
    OpenHtmlLogFile("Akinator.html");

    Tree tree = {};
    TreeCtor(&tree);
    GetTreeFromFile(&tree, DEFAULT_TREE_NAME);

    RunAkinator(&tree);
    
    SaveTreeInFile(&tree, DEFAULT_TREE_NAME);
    TreeDtor(&tree);

    CloseHtmlLogFile();
}