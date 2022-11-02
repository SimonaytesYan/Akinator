#ifndef __SYM_AKINATOR__
#define __SYM_AKINATOR__

#include <stdio.h>

#include "Libs/Logging/Logging.h"

const size_t MAX_CRIT_SIZE = 64;

typedef char Node_t[MAX_CRIT_SIZE];

const char DEFAULT_TREE_NAME[] = "Tree";

void PrintElemInLog(char val[MAX_CRIT_SIZE]);

#include "Libs/BinaryTree/BinaryTree.h"

int  SaveTreeInFile(Tree* tree, const char file_name[]);

int  GetTreeFromFile(Tree* tree, const char file_name[]);

bool Akinate(Tree* tree, Node* node);

void InitAkinator(Tree* tree);

void DeInitAkinator(Tree* tree);

void GetNodeFromFile(Node* node, void* fp_void);

int StrCmpDifferenCases(const char* s1, const char* s2);

bool GetObjCriteria(Tree* tree, Node* node, char object[MAX_CRIT_SIZE]);

void RunAkinator();

#endif //__SYM_AKINATOR__