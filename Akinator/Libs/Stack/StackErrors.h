#ifndef __STACK_ERRORS_SYM__
#define __STACK_ERRORS_SYM__

enum StackErrorCodes
{
    NO_ERROR_STK                = 0,
    NULL_STACK_POINTER_STK      = 1,
    MEMORY_ALLOCATION_ERROR_STK = 1<<1,
    WRONG_SIZE_STK              = 1<<2,
    WRONG_CAPACITY_STK          = 1<<3,
    SIZE_BIGGER_CAPACITY_STK    = 1<<4,
    NULL_DATA_STK               = 1<<5,
    ERROR_LOGS_OPEN_STK         = 1<<6,
    LEFT_BORDER_DAMAGED_STK     = 1<<7,
    RIGHT_BORDER_DAMAGED_STK    = 1<<8,
    STRUCT_HASH_MISMATCH_STK    = 1<<9,
    DATA_HASH_MISMATCH_STK      = 1<<10,
    DEBUG_INFO_DAMAGED_STK      = 1<<11,
};

const char* const ERROR_DESCRIPTION_STACK[] = { "Pointer to stack = nullptr\n",
                                    "Error during memmory allocation\n",
                                    "Negative or poison size\n",
                                    "Negative or poison capacity\n",
                                    "Size is bigger then capacity\n",
                                    "Pointer to stack.data = nullptr\n",
                                    "Error during open logs file\n",
                                    "The left boundary element is damaged. Other data in the structure may have been changed\n",
                                    "The right boundary element is damaged. Other data in the structure may have been changed\n",
                                    "Structure of stack was damaged\n",
                                    "Stack elements was damaged\n",
                                    "Bebug info from stack damaged"};
#endif  //__STACK_ERRORS_SYM__