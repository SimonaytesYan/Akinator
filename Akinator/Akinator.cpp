#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>

#include "Akinator.h"
#include "Libs/ConsoleSettings/ConsoleSettings.h"

typedef Node* StackElem;

#include "Libs/Stack/Stack.h"

#ifdef _WIN32

    #include <stringapiset.h>

    #define TX_USE_SPEAK 
    #include "Libs/TXLib.h"

#endif

//#define DEBUG

#define VOICE_COMMANDS

const int   MAX_STRLEN_IN_PRINT = 1000;
const char  VOICE_COMAND_PROTOTIPE[] = "echo \"%s\" | festival --language russian --tts 2>console.log";
const int   DENIALS_NUMBER           = 6;
const char* DENIALS[]                = {"не",
                                        "вообще не",
                                        "ни сколько не",
                                        "совсем не",
                                        "не в каком месте не",
                                        "ну вообще не",
                                        };

#ifdef _WIN32 

    static int ConvertUTF8To1251AndPrintf(const char *const format, ...);

    static int  ConvertUTF8To1251(const char* const str_UTF8, char* const str_1251);

    #define printf(_Format, ...) ConvertUTF8To1251AndPrintf(_Format, ##__VA_ARGS__)
    
    static int ConvertUTF8To1251(const char* const str_UTF8, char* const str_1251)
    {
        wchar_t UTF16[MAX_STRLEN_IN_PRINT] = L"";
        
        MultiByteToWideChar(CP_UTF8, 0, str_UTF8, -1, UTF16,    MAX_STRLEN_IN_PRINT);
        WideCharToMultiByte(CP_ACP,  0, UTF16,    -1, str_1251, MAX_STRLEN_IN_PRINT, NULL, NULL);
        
        return 0;
    }

    static int ConvertUTF8To1251AndPrintf(const char *const format, ...)
    {
        va_list args;
        va_start(args, format);
        
        char outputUTF8 [MAX_STRLEN_IN_PRINT] = "";
        char output1251 [MAX_STRLEN_IN_PRINT] = "";

        vsprintf(outputUTF8, format, args);
        
        ReturnIfError(ConvertUTF8To1251(outputUTF8, output1251));

        fprintf(stdout, "%s", output1251);

        return 0;
    }

#endif

bool Akinate(Tree* tree, Node* node, Stack* stack = nullptr);

static void GetLine(FILE* fp, char string[], int num);

static void PrintWithVoiceInCmd(const char *const format, ...);

static int  CleanBuffer();

static void PrintRandDenialCmd();

static void PrintOperationMode();

static void PrintRandDenialCmd()
{
    PrintWithVoiceInCmd(DENIALS[rand()%DENIALS_NUMBER]);
}

static void PrintWithVoiceInCmd(const char *const format, ...)
{
    va_list args;
    va_start(args, format);

    char output[MAX_STRLEN_IN_PRINT] = "";
    vsprintf(output, format, args);

    printf("%s", output);

    #ifdef VOICE_COMMANDS

        #ifdef _WIN32
            ConvertUTF8To1251(output, output);
            txSpeak("%s", output);
        #else
            char comand[MAX_CRIT_SIZE * MAX_CRIT_SIZE + MAX_CRIT_SIZE] = "";
            sprintf(comand, VOICE_COMAND_PROTOTIPE, output);

            #ifdef DEBUG
                printf(comand);
            #endif
            
            system(comand);
        #endif

    #endif

    va_end(args);
}

static int CleanBuffer()
{
	int cnt = 0;
	while ((getchar()) != '\n') 
		cnt++;
	return cnt;
}

void KnowObjDiff(Tree* tree)
{
    PrintWithVoiceInCmd("Введи объекты, которые ты хочешь сравнить\n");\
    char object1[MAX_CRIT_SIZE] = "";
    char object2[MAX_CRIT_SIZE] = "";
    
    long long mask1 = 0;
    long long mask2 = 0;

    PrintWithVoiceInCmd("Первый объект: ");
    while (true)
    {
        GetLine(stdin, object1, MAX_CRIT_SIZE);
        mask1 = GetMaskObjCritetia(tree, tree->root, object1, 0);

        if (mask1 != 0)
            break;

        PrintWithVoiceInCmd("Ты ошибся. Этого объекта не существует. Введи ещё раз\n");
    }

    PrintWithVoiceInCmd("Второй объект: ");
    while (true)
    {
        GetLine(stdin, object2, MAX_CRIT_SIZE);
        mask2 = GetMaskObjCritetia(tree, tree->root, object2, 0);
        
        if (mask2 != 0)
            break;

        PrintWithVoiceInCmd("Ты ошибся. Этого объекта не существует. Введи ещё раз\n");
    }

    #ifdef DEBUG
        printf("obj1 = <%s>\n", object1);
        printf("obj2 = <%s>\n", object2);
    #endif
    
    printf("**********************************\n");
    ChangeColor(stdout, TURQUOISE);
    PrintWithVoiceInCmd("%s", object1);
    ChangeColor(stdout, DEFAULT_COLOR);
    PrintWithVoiceInCmd(" и ");
    ChangeColor(stdout, TURQUOISE);
    PrintWithVoiceInCmd("%s", object2);
    ChangeColor(stdout, DEFAULT_COLOR);
    PrintWithVoiceInCmd(" похожи тем, что и тот и другой\n");

    PrintDiffMaskCriteria(tree, tree->root, mask1, mask2, 0, object1, object2);
    printf("**********************************\n");
    
    printf("\n");
}

void PrintDiffMaskCriteria(Tree* tree, Node* node, int64_t mask1, int64_t mask2, int64_t height, char obj1[], char obj2[])
{
    if (node == nullptr)
        return;

    if ((mask1 & (1 << height)) != ((mask2) & (1 << height)))
    {
        printf("\n");
        PrintWithVoiceInCmd("Объект ");
        ChangeColor(stdout, TURQUOISE);
        PrintWithVoiceInCmd("%s", obj1);
        ChangeColor(stdout, DEFAULT_COLOR);
        PrintWithVoiceInCmd(" отличается от ");

        ChangeColor(stdout, TURQUOISE);
        PrintWithVoiceInCmd("%s", obj2);
        ChangeColor(stdout, DEFAULT_COLOR);
        
        PrintWithVoiceInCmd(" тем, что ");
        
        ChangeColor(stdout, TURQUOISE);
        PrintWithVoiceInCmd("%s", obj1);
        ChangeColor(stdout, DEFAULT_COLOR);

        PrintWithVoiceInCmd(" - ");
        PrintMaskCriteria(node,  mask1, height);

        PrintWithVoiceInCmd("\n" "Зато ");

        ChangeColor(stdout, TURQUOISE);
        PrintWithVoiceInCmd("%s", obj2);
        ChangeColor(stdout, DEFAULT_COLOR);

        PrintWithVoiceInCmd(", в отличие от ");

        ChangeColor(stdout, TURQUOISE);
        PrintWithVoiceInCmd("%s ", obj1);
        ChangeColor(stdout, DEFAULT_COLOR);

        PrintMaskCriteria(node,  mask2, height);
        return;
    }

    if (mask1 & (1 << height))
    {
        PrintWithVoiceInCmd(" %s; ", node->val);

        PrintDiffMaskCriteria(tree, node->left, mask1, mask2, height + 1, obj1, obj2);
    }
    else
    {
        PrintRandDenialCmd();
        PrintWithVoiceInCmd(" %s; ", node->val);
        
        PrintDiffMaskCriteria(tree, node->right, mask1, mask2, height + 1, obj1, obj2);
    }
}

void DetermineObject(Tree* tree)
{
    PrintWithVoiceInCmd("Введи объект, определение которого ты хочешь узнать\n");\
    char object[MAX_CRIT_SIZE] = "";
    
    GetLine(stdin, object, MAX_CRIT_SIZE);
    #ifdef DEBUG
        printf("obj = <%s>\n", object);
    #endif

    long long mask = GetMaskObjCritetia(tree, tree->root, object, 0);

    if (mask == 0)
    {
        PrintWithVoiceInCmd("Это не постижимо! ");
        PrintWithVoiceInCmd("Объект не найден в моей базе! Ты ");
        
        ChangeColor(stdout, RED);
        PrintWithVoiceInCmd("жулик!\n");
        ChangeColor(stdout, DEFAULT_COLOR);
    }
    else
    {
        ChangeColor(stdout, TURQUOISE);        
        PrintWithVoiceInCmd("%s", object);
        ChangeColor(stdout, DEFAULT_COLOR);
        PrintWithVoiceInCmd(" - это");
        PrintMaskCriteria(tree->root, mask, 0);
    }
    printf("\n");
}

int StrCmpDifferenCases(const char* s1, const char* s2)
{
    #ifdef _WIN32
        return stricmp(s1, s2);
    #else
        return strcasecmp(s1, s2);
    #endif
}

void PrintElemInLog(char val[MAX_CRIT_SIZE])
{
    LogPrintf(val);
}

int SaveTreeInFile(Tree* tree, const char file_name[])
{
    ReturnIfError(TreeCheck(tree));

    FILE* fp = fopen(file_name, "w");
    CHECK(fp == nullptr, "Error during open file", -1);

    DFS_f pre_function = [](Node* node, void* dfs_fp)
                       {
                            fprintf((FILE*)dfs_fp, " { ");
                            fprintf((FILE*)dfs_fp, "%s", node->val);
                       };
    DFS_f post_function = [](Node*, void* dfs_fp)
                        {
                            fprintf((FILE*)dfs_fp, " } \n");
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

    while((c = getc(fp)) == ' ') 
    {
        if (c == EOF)
            return;
    };
    ungetc(c, fp);

    int i = 0;
    while ((c = getc(fp)) != '{' && c != EOF && c != '}')
    {
        node->val[i] = (char)c;
        i++;
    }

    i--;
    while (node->val[i] == ' ')
    {
        node->val[i] = '\0';
        i--;
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

long long GetMaskObjCritetia(Tree* tree, Node* node, char object[MAX_CRIT_SIZE], long long h)
{
    if (node == nullptr)
        return 0;
    
    if (node->left == nullptr && node->right == nullptr)
    {
        #ifdef DEBUG
            printf("leaf    = <%s>\n", node->val);
            int cmp_res = StrCmpDifferenCases(object, node->val);
            printf("cmp_res = %d\n", cmp_res); 
        #endif

        if (StrCmpDifferenCases(object, node->val) == 0)
        {
            int answer = 1 << h;
            return answer;
        }
        return 0;
    }

    long long result = GetMaskObjCritetia(tree, node->left, object, h + 1);

    if (result != 0)
    {
        result |= 1 << h;
        return result;
    }
    result = GetMaskObjCritetia(tree, node->right, object, h + 1);

    return result;
}

void PrintMaskCriteria(Node* node, long long mask, long long h)
{
    if (node == nullptr)
        return;
    
    if (mask & (1 << h))
    {
        PrintWithVoiceInCmd(" %s;", node->val);
        PrintMaskCriteria(node->left, mask, h + 1);
    }
    else
    {
        PrintWithVoiceInCmd(" ");
        PrintRandDenialCmd();
        PrintWithVoiceInCmd(" %s;", node->val);
        PrintMaskCriteria(node->right, mask, h + 1);
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

static void GetLine(FILE* fp, char str[], int num)
{
    char* new_line_symb = nullptr;
    do 
    {
        fgets(str, num, fp);
        new_line_symb = strchr(str, '\n');
        if (new_line_symb != nullptr)
            *new_line_symb = '\0';
    }
    while(new_line_symb == nullptr || strlen(str) == 0);
    
    #ifdef DEBUG
        int i = 0;
        while(str[i] != '\0')
        {
            printf("<%>%d", str[i], str[i]);
            i++;
        }
    #endif
}

void AddNewObjectAndCrit(Tree* tree, Node* node)
{
    char new_obj[MAX_CRIT_SIZE] = "";
    GetLine(stdin, new_obj, MAX_CRIT_SIZE);
                
    PrintWithVoiceInCmd("Чем %s отличается от %s?\n", new_obj, node->val);

    char new_criteria[MAX_CRIT_SIZE] = "";
    GetLine(stdin, new_criteria, MAX_CRIT_SIZE);
                
    #ifdef DEBUG
        printf("new_obj      = <%s>\n", new_obj);
        printf("new_criteria = <%s>\n", new_criteria);
    #endif

    TreeInsertLeafLeft (tree, new_obj,   node);
    TreeInsertLeafRight(tree, node->val, node);
    
    strcpy(node->val, new_criteria);
}

static bool GoYes(Tree* tree, Node* node, Stack* stack)
{
    if (node->left == nullptr)
    {
        PrintWithVoiceInCmd("Это ");
        
        ChangeColor(stdout, TURQUOISE);
        PrintWithVoiceInCmd("%s. ", node->val);
        ChangeColor(stdout, DEFAULT_COLOR);
        
        PrintWithVoiceInCmd("Это было легко!!!\n");

        StackDtor(stack);
        free(stack);

        return true;
    }
    return Akinate(tree, node->left, stack);
}

static bool GoNo(Tree* tree, Node* node, Stack* stack)
{
    if (node->right == nullptr)
    {
        if (stack->size != 0)
        { 
            Node* next_node = StackPop(stack);

            return Akinate(tree, next_node, stack);
        }

        StackDtor(stack);
        free(stack);

        PrintWithVoiceInCmd("Наконец-то достойный соперник! ");
        PrintWithVoiceInCmd("Наше стражение стало легендарным!\n");
        PrintWithVoiceInCmd("Кого ты загадал?\n");

        AddNewObjectAndCrit(tree, node);

        PrintWithVoiceInCmd("Теперь я стал сильнее! ");
        PrintWithVoiceInCmd("В этот раз тебе не удасться победить!!!\n");

        return true;
    }

    return Akinate(tree, node->right, stack);
}

bool Akinate(Tree* tree, Node* node, Stack* stack)
{
    if (node == nullptr)
        return false;

    if (stack == nullptr)
    {
        stack = (Stack*)calloc(sizeof(Stack), 1);
    }

    assert(tree);

    ChangeColor(stdout, YELLOW);
    PrintWithVoiceInCmd("%s?\n", node->val);
    ChangeColor(stdout, DEFAULT_COLOR);

    int answer = 0;
    while(true)
    {
        printf("**********************************\n");
        printf("Ваш ответ:\n");
        printf("1 - да\n");
        printf("2 - возможно да\n");
        printf("3 - возможно нет\n");
        printf("4 - нет\n");
        printf("5 - выход\n");

        scanf("%d", &answer);

        #ifdef DEBUG
            printf("ans = <%s>\n", answer);
        #endif

        switch(answer)
        {
            case 1:
            {
                return GoYes(tree, node, stack);
                break;
            }
            case 2:
            {
                StackPush(stack, node->right);
                return GoYes(tree, node, stack);
                break;
            }
            case 3:
            {
                StackPush(stack, node->left);
                return GoNo(tree, node, stack);
                break;
            }
            case 4:
            {
                return GoNo(tree, node, stack);
                break;
            }
            case 5:
            {
                StackDtor(stack);
                free(stack);
                return false;
                break;
            }
            default:
            {
                PrintWithVoiceInCmd("Неизвестная команда. Повторите ввод\n");
                break;
            }
        }
    }
}

void OutputGraphicDump(Tree* tree)
{

    GraphicDump(tree);

    char graphic_dump_file[70] = "";
    #ifdef _WIN32
        sprintf(graphic_dump_file, ".\\GraphicDumps\\Dump%d.png", GRAPHIC_DUMP_CNT - 1);
    #else
        sprintf(graphic_dump_file, "xdg-open GraphicDumps/Dump%d.png", GRAPHIC_DUMP_CNT - 1);
    #endif

    system(graphic_dump_file);
}

static void PrintOperationMode()
{
    printf("**********************************\n");
    printf("Режимы:\n");
    ChangeColor(stdout, GREEN);
    printf("-1");
    ChangeColor(stdout, DEFAULT_COLOR);
    printf(" - Выход из программы\n"); 
    ChangeColor(stdout, GREEN);
    printf("0");
    ChangeColor(stdout, DEFAULT_COLOR);
    printf("  - Угадать\n");
    ChangeColor(stdout, GREEN);
    printf("1");
    ChangeColor(stdout, DEFAULT_COLOR);
    printf("  - Дать определение\n");
    ChangeColor(stdout, GREEN);
    printf("2");
    ChangeColor(stdout, DEFAULT_COLOR);
    printf("  - Сравнение\n");
    ChangeColor(stdout, GREEN);
    printf("3");
    ChangeColor(stdout, DEFAULT_COLOR);
    printf("  - Графический дамп\n");
    printf("**********************************\n");
}

void RunAkinator(Tree* tree)
{
    while(true)
    {
        PrintOperationMode();

        int operation_mode = -2;
        
        while (operation_mode < -1 || operation_mode > 3)
        {
            PrintWithVoiceInCmd("В каком режиме запустить программу?\n");
            scanf("%d", &operation_mode);
            CleanBuffer();
            #ifdef DEBUG
                printf("Oper_mode = %d\n", operation_mode);
            #endif
        }

        bool end_program = false;
        switch (operation_mode)
        {
            case -1:
                end_program = true;
                break;
            case 0:
            {
                Akinate(tree, tree->root);
                break;
            }

            case 1:
            {
                DetermineObject(tree);
                break;
            }
            
            case 2:
            {
                KnowObjDiff(tree);
                break;
            }
            
            case 3:
            {
                OutputGraphicDump(tree);
                break;
            }
            default:
                PrintWithVoiceInCmd("Интересный у тебя operation mode, конечно...\n");
                break;
        }

        if (end_program)
            break;

        sleep(1);
    }

    PrintWithVoiceInCmd("Спасибо за игру!\n");
}