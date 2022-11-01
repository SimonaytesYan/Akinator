#include "Akinator.h"
#include <stdio.h>

void PrintElemInLog(char val[MAX_CRIT_SIZE])
{
    LogPrintf(val);
}

int SaveTreeInFile(Tree* tree, const char file_name[])
{
    printf("Save tree\n");
    ReturnIfError(TreeCheck(tree));

    FILE* fp = fopen(file_name, "w");
    CHECK(fp == nullptr, "Error during open file", -1);

    DFS_f pre_function = [](Node* node, void* fp)
                       {
                            fprintf((FILE*)fp, " { ");
                            fprintf((FILE*)fp, node->val);
                       };
    DFS_f post_function = [](Node* node, void* fp)
                        {
                            fprintf((FILE*)fp, " } ");
                        };

    DFS(tree->root, pre_function,  fp,
                    nullptr,       nullptr,
                    post_function, fp);

    fclose(fp);

    return 0;
}
