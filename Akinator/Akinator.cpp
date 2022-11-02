#include "Akinator.h"
#include <stdio.h>

void PrintElemInLog(char val[MAX_CRIT_SIZE])
{
    LogPrintf(val);
}

int SaveTreeInFile(Tree* tree, const char file_name[])
{
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
                            fprintf((FILE*)fp, " } \n");
                        };

    DFS(tree->root, pre_function,  fp,
                    nullptr,       nullptr,
                    post_function, fp);

    fclose(fp);

    return 0;
}

void GetNodeFromFile(Node* node, void* fp_void)
{
    FILE* fp = (FILE*)fp_void;

    int c = getc(fp);
    while(c != '{' && c != EOF)
        c = getc(fp);

    if (c == EOF) return;

    int i = 0;
    while ((c = getc(fp)) != '{' && c != EOF && c != '}')
    {
        node->val[i] = c;
        i++;
    }

    if (c == EOF) return;

    if (c == '}')
    {
        node->left  = nullptr;
        node->right = nullptr;
    }
    else
    {
        ungetc('{', fp);
        node->left  = (Node*)calloc(sizeof(Node), 1);
        node->right = (Node*)calloc(sizeof(Node), 1);
    }
}

int GetTreeFromFile(Tree* tree, const char file_name[])
{
    ReturnIfError(TreeCheck(tree));

    FILE* fp = fopen(file_name, "r");
    CHECK(fp == nullptr, "Error during open file", -1);

    DFS(tree->root, GetNodeFromFile, fp,
                    nullptr,         nullptr,
                    nullptr,         nullptr);

    fclose(fp);

    return 0;
}
