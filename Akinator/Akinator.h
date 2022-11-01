#ifndef __SYM_AKINATOR__
#define __SYM_AKINATOR__

#include <stdio.h>

#include "Libs\Logging\Logging.h"

const size_t MAX_CRIT_SIZE = 64;

typedef char Node_t[MAX_CRIT_SIZE];

void PrintElemInLog(char val[MAX_CRIT_SIZE]);

#include "Libs\BinaryTree\BinaryTree.h"

int SaveTreeInFile(Tree* tree, const char file_name[]);

#endif //__SYM_AKINATOR__