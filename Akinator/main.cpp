#include <stdio.h>

#include "Akinator.h"

int main()
{
    Tree tree = {};

    TreeCtor(&tree);

    strcpy(tree.root->val, "Животное");

     TreeInsertLeafLeft(&tree, "1,5", tree.root); 
     TreeInsertLeafRight(&tree, "Ведёт матан", tree.root);

     TreeInsertLeafLeft(&tree, "Математик", tree.root->right); 
     TreeInsertLeafRight(&tree, "Поёт", tree.root->right);
     
     TreeInsertLeafLeft(&tree, "Алла Пугачёва", tree.root->right->right); 
     TreeInsertLeafRight(&tree, "Дед", tree.root->right->right);

    GraphicDump(&tree);
}