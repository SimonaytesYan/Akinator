#ifndef __SYM_BINARY_TREE__
#define __SYM_BINARY_TREE__

#include <stdlib.h>
#include <string.h>

#include "..\Logging\Logging.h"
#include "..\Errors.h"


static const char   COMAND_PROTOTYPE[] = "Dot GraphicDumps/dump%d -o GraphicDumps/Dump%d.png -T png";
static       int    GRAPHIC_DUMP_CNT   = 0;

typedef struct Node 
{
    Node_t val   = "";
    Node*  left  = nullptr;
    Node*  right = nullptr;
    Node*  prev  = nullptr;
} Node;

typedef void(*DFS_f)(Node* node, void*);

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

static int  TreeCheck(Tree* tree);

static int  TreeConstructor(Tree* tree, int capacity, int line, const char* name, const char* function, const char* file);

static int  TreeDtor(Tree* tree);

static void DumpTree(Tree* tree, const char* function, const char* file, int line);

static void GraphicDump(Tree* tree);

static int  TreeInsertLeafRight(Tree* tree, const Node_t value, Node* after_which);

static int  TreeInsertLeafLeft(Tree* tree, const Node_t value, Node* after_which);

static int  TreeIterate(Tree* tree, Node** index);

static void GraphicNodeDump(Node* node, FILE* fp);

static void GraphicDump(Tree* tree);

static void DumpNode(Node* node);

static void DFS(Node* node, DFS_f f1, void* args1, DFS_f f2, void* args2, DFS_f f3, void* args3)
{
    if (node == nullptr)
        return;

    if (f1 != nullptr)
        f1(node, args1);

    DFS(node->left, f1, args1, f2, args2, f3, args3);

    if (f2 != nullptr)
        f2(node, args2);
    
    DFS(node->right, f1, args1, f2, args2, f3, args3);

    if (f2 != nullptr)
        f3(node, args3);
}

static void GraphicNodeDump(Node* node, FILE* fp)
{
    if (node == nullptr)
        return;

    fprintf(fp, "Node%06X[style = \"filled,rounded\", fillcolor = \"#B1FF9F\", label = \"{<i>%06X \\n | <v>%s \\n | { <l> %06X |<r>  %06X}}\"]\n",
                    node,                                                    node,      node->val,   node->left,   node->right);
    
    if (node->left != nullptr)
        fprintf(fp, "Node%X -> Node%X[xlabel = \"Да\"]\n", node, node->left);
    if (node->right != nullptr)
        fprintf(fp, "Node%X -> Node%X[xlabel = \"Нет\"]\n", node, node->right);

    GraphicNodeDump(node->left, fp);
    GraphicNodeDump(node->right, fp);
}

static void GraphicDump(Tree* tree)
{
    char name[30] = "";
    sprintf(name, "GraphicDumps/dump%d", GRAPHIC_DUMP_CNT);
    FILE* fp = fopen(name, "w");

    fprintf(fp, "digraph G{\n");
    fprintf(fp, "node[shape = record, fontsize = 14];\n");
    fprintf(fp, "splines = ortho\n");

    if (tree == nullptr) 
        return;
    if (tree->root == nullptr)
        return;

    fprintf(fp, "info[label = \"root = 0x%06X\"]\n", tree->root);

    GraphicNodeDump(tree->root, fp);
    
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

static void DumpNode(Node* node)
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

static void DumpTree(Tree* tree, const char* function, const char* file, int line)
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

static int TreeCheck(Tree* tree)
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

#define TreeCtor(tree)  TreeConstructor(tree, __LINE__, #tree, __PRETTY_FUNCTION__, __FILE__)

static int TreeConstructor(Tree* tree, int line, const char* name, const char* function, const char* file)
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

static void NodeDtor(Node* node)
{
    if (node == nullptr)
        return;

    NodeDtor(node->left);
    NodeDtor(node->right);

    node->left  = nullptr;
    node->right = nullptr;

    free(node);
}

static int TreeDtor(Tree* tree)
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

static int TreeInsertLeafRight(Tree* tree, const Node_t value, Node* after_which)
{
    ReturnIfError(TreeCheck(tree));
    CHECK(after_which == nullptr, "after_which = nullptr", -1);

    Node* new_elem = (Node*)calloc(1, sizeof(Node));
    strcpy(new_elem->val, value);

    after_which->right = new_elem;

    return 0;
}

static int TreeInsertLeafLeft(Tree* tree, const Node_t value, Node* after_which)
{
    ReturnIfError(TreeCheck(tree));
    CHECK(after_which == nullptr, "after_which = nullptr", -1);

    Node* new_elem = (Node*)calloc(1, sizeof(Node));
    strcpy(new_elem->val, value);

    after_which->left = new_elem;

    return 0;
}

#endif //__SYM_BINARY_TREE__