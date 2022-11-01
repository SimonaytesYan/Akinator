#ifndef __SYM_BINARY_TREE__
#define __SYM_BINARY_TREE__

#include <stdlib.h>
#include <string.h>

#include "Logging\Logging.h"
#include "Errors.h"

const char   COMAND_PROTOTYPE[] = "Dot GraphicDumps/dump%d -o GraphicDumps/Dump%d.png -T png";
      int    GRAPHIC_DUMP_CNT   = 0;

typedef struct Node 
{
    Node_t val   = "";
    Node*  left  = nullptr;
    Node*  right = nullptr;
    Node*  prev  = nullptr;
} Node;

typedef struct LogInfo 
{
    const char* name     = "(null)";
    const char* function = "(null)";
    const char* file     = "(null)";
    int         line     = -1;
    bool        status   = false;
} LogInfo;

typedef struct Tree 
{
    Node*     root = nullptr;
    LogInfo   debug     = {};
}Tree;

int  TreeCheck(Tree* tree);

int  TreeConstructor(Tree* tree, int capacity, int line, const char* name, const char* function, const char* file);

int  TreeDtor(Tree* tree);

void DumpTree(Tree* tree, const char* function, const char* file, int line);

void GraphicDump(Tree* tree);

int  TreeInsert(Tree* tree, Node_t value, Node* after_which, Node** index = nullptr);

int  TreeIterate(Tree* tree, Node** index);

void GraphicNodeDump(Node* node, FILE* fp)
{
    if (node == nullptr)
        return;

    fprintf(fp, "Node%06X[style = \"filled,rounded\", fillcolor = \"#8F9EFF\", label = \"<i>%06X \\n | <v>%s \\n | {<l> %06X |<r>  %06X}\"]\n",
                    node,                                                    node,      node->val,   node->left,   node->right);
    
    if (node->left != nullptr)
        fprintf(fp, "Node%X -> Node%X[color = \"blue\"]\n", node, node->left);
    if (node->right != nullptr)
        fprintf(fp, "Node%X -> Node%X[color = \"red\"]\n", node, node->right);

    GraphicNodeDump(node->left, fp);
    GraphicNodeDump(node->right, fp);
}


void GraphicDump(Tree* tree)
{
    char name[20] = "";
    sprintf(name, "GraphicDumps/dump%d", GRAPHIC_DUMP_CNT);
    FILE* fp = fopen(name, "w");

    fprintf(fp, "digraph G{\n");
    fprintf(fp, "node[shape = record, fontsize = 14];\n");
    fprintf(fp, "splines = ortho\n");

    if (tree == nullptr) 
        return;
    if (tree->root == nullptr)
        return;

    fprintf(fp, "info[label = \"root = 0x06%X\"]\n", tree->root);

    Node* root = tree->root;
    GraphicNodeDump(tree->root->left, fp);
    GraphicNodeDump(tree->root->right, fp);
    
    fprintf(fp, "}");

    fclose(fp);

    char comand[50] = "";
    sprintf(comand, COMAND_PROTOTYPE, GRAPHIC_DUMP_CNT, GRAPHIC_DUMP_CNT);
    system(comand);
    
    char path[50] = "";
    LogPrintf("<hr>");
    LogPrintf("<img src=\"GraphicDumps/Dump%d.png\">\n", GRAPHIC_DUMP_CNT);
    LogPrintf("<hr>");

    GRAPHIC_DUMP_CNT++;
}

void DumpNode(Node* node)
{
    if (node == nullptr)
        return;

    LogPrintf("(");
    PrintElemInLog(node->val);
    
    DumpNode(node->left);
    DumpNode(node->right);

    LogPrintf(")");
}

#define DUMP_L(tree) DumpTree(tree, __PRETTY_FUNCTION__, __FILE__, __LINE__)

void DumpTree(Tree* tree, const char* function, const char* file, int line)
{
    LogPrintf("\nDump in %s(%d) in function %s\n", file, line, function);

    TreeCheck(tree);
    
    if (tree == nullptr) 
        return;

    LogPrintf("Stack[%p] ", tree);

    if (tree->debug.name != nullptr)
        LogPrintf("\"%s\"", tree->debug.name);
    if (tree->debug.function != nullptr)
        LogPrintf("created at %s ", tree->debug.function);
    if (tree->debug.file != nullptr)
        LogPrintf("at %s(%d):", tree->debug.file);
    LogPrintf("\n");

    LogPrintf("Status: ");
    if (tree->debug.status)
        LogPrintf("enable\n");
    else
        LogPrintf("disable\n");

    LogPrintf("{\n");

    DumpNode(tree->root);

    LogPrintf("}\n\n");
}

int TreeCheck(Tree* tree)
{
    int error = 0;
    if (tree == nullptr)
        error |= NULL_TREE_POINTER;
    else
    {
        if (tree->root == nullptr)
            error != NULL_POINTER_TO_ROOT;
        
        if (tree->debug.file     == nullptr) error |= DEBUG_FILE_DAMAGED;
        if (tree->debug.function == nullptr) error |= DEBUG_FUNCTION_DAMAGED;
        if (tree->debug.name     == nullptr) error |= DEBUG_NAME_DAMAGED;
    }

    LogAndParseErr(error != 0, error);
    
    return error;
}

#define TreeCtor(tree, capacity)  TreeConstructor(tree, capacity, __LINE__, #tree, __PRETTY_FUNCTION__, __FILE__)

int TreeConstructor(Tree* tree, int capacity, int line, const char* name, const char* function, const char* file)
{
    LogAndParseErr(tree == nullptr, NULL_TREE_POINTER);

    tree->root           = (Node*)calloc(1, sizeof(Node));
    tree->root->left     = tree->root;
    tree->root->right    = tree->root;

    tree->debug.name     = name;
    tree->debug.function = function;
    tree->debug.file     = file;
    tree->debug.line     = line;
    tree->debug.status   = true;

    return TreeCheck(tree);
}

void NodeDtor(Node* node)
{
    if (node == nullptr)
        return;

    NodeDtor(node->left);
    NodeDtor(node->right);

    node->left  = nullptr;
    node->right = nullptr;

    free(node);
}

int TreeDtor(Tree* tree)
{
    TreeCheck(tree);

    Node* index = tree->root;

    NodeDtor(tree->root);
    tree->root = nullptr;

    tree->debug.file     = nullptr;
    tree->debug.function = nullptr;
    tree->debug.name     = nullptr;
    tree->debug.line     = 0;

    return 0;
}

int TreeInsertListRight(Tree* tree, const Node_t value, Node* after_which)
{
    ReturnIfError(TreeCheck(tree));
    CHECK(after_which == nullptr, "after_which = nullptr", -1);

    Node* new_elem = (Node*)calloc(1, sizeof(Node));
    strcpy(new_elem->val, value);

    after_which->right = new_elem;

    return 0;
}

int TreeInsertListLeft(Tree* tree, const Node_t value, Node* after_which)
{
    ReturnIfError(TreeCheck(tree));
    CHECK(after_which == nullptr, "after_which = nullptr", -1);

    Node* new_elem = (Node*)calloc(1, sizeof(Node));
    strcpy(new_elem->val, value);

    after_which->left = new_elem;

    return 0;
}



#endif //__SYM_BINARY_TREE__