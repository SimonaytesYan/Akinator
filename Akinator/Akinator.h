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

void GetNodeFromFile(Node* node, void* fp_void);

int StrCmpDifferenCases(const char* s1, const char* s2);

void PrintMaskCriteria(Node* node, long long mask, long long h);

void AddNewObjectAndCrit(Tree* tree, Node* node);

long long GetMaskObjCritetia(Tree* tree, Node* node, char object[MAX_CRIT_SIZE], long long h);

void KnowObjDiff(Tree* tree);

void OutputGraphicDump(Tree* tree);

void GuessObject(Tree* tree);

void RunAkinator(Tree* tree);

void PrintDiffMaskCriteria(Tree* tree, Node* node, int64_t mask1, int64_t mask2, int64_t h, char obj1[], char obj2[]);

#endif //__SYM_AKINATOR__