#ifndef __TREE_ERRORS_SYM__
#define __TREE_ERRORS_SYM__

const int ERRORS_NUMBER = 9;

enum ErrorCodes
{
    NO_ERROR                     = 0,
    NULL_TREE_POINTER            = 1<<0,
    MEMORY_ALLOCATION_ERROR      = 1<<1,
    ERROR_LOGS_OPEN              = 1<<2,
    WRONG_SIZE                   = 1<<3,
    WRONG_CAPACITY               = 1<<4,
    DEBUG_FILE_DAMAGED           = 1<<5,
    DEBUG_FUNCTION_DAMAGED       = 1<<6,
    DEBUG_LINE_DAMAGED           = 1<<7,
    DEBUG_NAME_DAMAGED           = 1<<8,
    NULL_POINTER_TO_ROOT         = 1<<9,
};

const char* const ERROR_DESCRIPTION[] = { "Pointer to tree = nullptr\n",
                                          "Error during memmory allocation\n",
                                          "Error during open logs\n",
                                          "Poisoned size\n",
                                          "Poisoned capacity\n",
                                          "Information about file in which tree created damaged\n",
                                          "Information about function in which tree created damaged\n",
                                          "Information about line in which tree created damaged\n",
                                          "Information about name of tree damaged\n",
                                          "Pointer to root = null\n"};
#endif  //__TREE_ERRORS_SYM__