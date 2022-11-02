#ifndef __SYM_BINARY_TREE__
#define __SYM_BINARY_TREE__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#include <stdlib.h>
#include <string.h>

#include "../Logging/Logging.h"
#include "../Errors.h"

static const char   COMAND_PROTOTYPE[] = "dot GraphicDumps/dump%d -o GraphicDumps/Dump%d.png -T png";
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

static void DFS(Node* node, DFS_f pre_func, void* args1, DFS_f in_func, void* args2, DFS_f post_func, void* args3);

static void DFS(Node* node, DFS_f pre_func, void* args1, DFS_f in_func, void* args2, DFS_f post_func, void* args3)
{
    if (node == nullptr)
        return;

    if (pre_func != nullptr)
        pre_func(node, args1);

    DFS(node->left, pre_func, args1, in_func, args2, post_func, args3);

    if (in_func != nullptr)
        in_func(node, args2);
    
    DFS(node->right, pre_func, args1, in_func, args2, post_func, args3);

    if (post_func != nullptr)
        post_func(node, args3);
}


static void WriteNodeAndEdge(Node* node, void* fp_void)
{ 
    FILE* fp = (FILE*)fp_void;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    fprintf(fp, "Node%06X[style = \"filled,rounded\", fillcolor = \"#B1FF9F\", label = \"{<i>%06X \\n | <v>%s \\n | { <l> %06X |<r>  %06X}}\"]\n",
                node,                                                                   node, node->val, node->left, node->right);
    
    if (node->left != nullptr)
        fprintf(fp, "Node%06X -> Node%06X[xlabel = \"Да\"]\n", node, node->left);
    if (node->right != nullptr)
        fprintf(fp, "Node%06X -> Node%06X[xlabel = \"Нет\"]\n", node, node->right);
    #pragma GCC diagnostic pop
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

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    fprintf(fp, "info[label = \"root = 0x%06X\"]\n", tree->root);
    #pragma GCC diagnostic pop

    DFS(tree->root, WriteNodeAndEdge, fp, 
                    nullptr,          nullptr,
                    nullptr,          nullptr);
    
    fprintf(fp, "}");

    fclose(fp);

    char comand[100] = "";
    sprintf(comand, COMAND_PROTOTYPE, GRAPHIC_DUMP_CNT, GRAPHIC_DUMP_CNT);
    system(comand);
    
    LogPrintf("<hr>");
    LogPrintf("<img src=\"GraphicDumps/Dump%d.png\">\n", GRAPHIC_DUMP_CNT);
    LogPrintf("<hr>");

    GRAPHIC_DUMP_CNT++;
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

    DFS_f pre_function = [](Node* node, void*)
                       { 
                           LogPrintf("(");
                           PrintElemInLog(node->val);
                       };
    DFS_f post_function = [](Node*, void*)
                        {
                            LogPrintf(")");
                        };

    DFS(tree->root, pre_function,  nullptr,
                    nullptr,       nullptr,
                    post_function, nullptr);

    LogPrintf("\n}\n\n");
}

static int TreeCheck(Tree* tree)
{
    int error = 0;
    if (tree == nullptr)
        error |= NULL_TREE_POINTER;
    else
    {
        if (tree->root == nullptr)
            error |= NULL_POINTER_TO_ROOT;
        
        if (tree->debug.file     == nullptr) error |= DEBUG_FILE_DAMAGED;
        if (tree->debug.function == nullptr) error |= DEBUG_FUNCTION_DAMAGED;
        if (tree->debug.name     == nullptr) error |= DEBUG_NAME_DAMAGED;
    }

    LogAndParseErr(error != 0, error);
    
    return error;
}

#define TreeCtor(tree) TreeConstructor(tree, __LINE__, #tree, __PRETTY_FUNCTION__, __FILE__)

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

static int TreeDtor(Tree* tree)
{
    TreeCheck(tree);
    
    DFS_f post_function = [](Node* node, void*)
                            {
                                node->left  = nullptr;
                                node->right = nullptr;
                            };

    DFS(tree->root, nullptr,       nullptr,
                    nullptr,       nullptr,
                    post_function, nullptr);
    
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

#pragma GCC diagnostic pop

#endif //__SYM_BINARY_TREE__