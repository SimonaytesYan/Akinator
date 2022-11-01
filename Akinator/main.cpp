#include <stdio.h>

#include "Libs/ConsoleSettings/ConsoleSettings.h"
#include "Akinator.h"

int main()
{
    OpenHtmlLogFile("Akinator.html");

    init_console();

    printf("Start\n");

    Tree tree = {};

    TreeCtor(&tree);

    GetTreeFromFile(&tree, "Tree");

    /*strcpy(tree.root->val, "Животное");

    TreeInsertLeafLeft(&tree, "1,5", tree.root); 
    TreeInsertLeafRight(&tree, "Ведёт матан", tree.root);

    TreeInsertLeafLeft(&tree, "Математик", tree.root->right); 
    TreeInsertLeafRight(&tree, "Поёт", tree.root->right);
     
    TreeInsertLeafLeft(&tree, "Алла Пугачёва", tree.root->right->right); 
    TreeInsertLeafRight(&tree, "Дед", tree.root->right->right);
*/
    DUMP_L(&tree);

    GraphicDump(&tree);

    SaveTreeInFile(&tree, "Tree");

    printf("End");
    CloseHtmlLogFile();
}