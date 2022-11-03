#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "Akinator.h"

#ifdef _WIN32
#include "Libs/TXLib.h"
#endif

//#define DEBUG

//!
//!Work 
//!
#define VOICE_COMMANDS

const char COMAND_PROTOTIPE[] = "echo \"%s\" | festival --language russian --tts";

static void GetLine(FILE* fp, char string[], int num);

static int  CleanBuffer();

static void PrintWithVoiceInCmd(const char *const format, ...)
{
    va_list args;
    va_start(args, format);

    char output[MAX_CRIT_SIZE * MAX_CRIT_SIZE] = "";
    vsprintf(output, format, args);

    #ifdef VOICE_COMMANDS
        char comand[MAX_CRIT_SIZE * MAX_CRIT_SIZE + MAX_CRIT_SIZE] = "";
        sprintf(comand, COMAND_PROTOTIPE, output);
        system(comand);
    #endif

    printf("%s", output);

    va_end(args);
}

int CleanBuffer()
{
	int cnt = 0;
	while ((getchar()) != '\n') 
		cnt++;
	return cnt;
}

void KnowObjDiff(Tree* tree)
{
    PrintWithVoiceInCmd("Введите объекты, которые ты хочешь сравнить\n");\
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

    PrintWithVoiceInCmd("%s и %s похожи тем, что ", object1, object2);

    PrintDiffMaskCriteria(tree, tree->root, mask1, mask2, 0, object1, object2);
    
    printf("\n");
}

void PrintDiffMaskCriteria(Tree* tree, Node* node, int64_t mask1, int64_t mask2, int64_t h, char obj1[], char obj2[])
{
    if (node == nullptr)
        return;

    if ((mask1 & (1 << h)) != ((mask2) & (1 << h)))
    {
        printf("\n");

        PrintWithVoiceInCmd("Объект %s отличается от %s тем, что %s - ", obj1, obj2, obj1);
        PrintMaskCriteria(node,  mask1, h);

        PrintWithVoiceInCmd("\n" "Зато %s, в отличие от %s ", obj2, obj1);
        PrintMaskCriteria(node,  mask2, h);
        return;
    }

    if (mask1 & (1 << h))
    {
        PrintWithVoiceInCmd(" %s;", node->val);

        PrintDiffMaskCriteria(tree, node->left, mask1, mask2, h + 1, obj1, obj2);
    }
    else
    {
        PrintWithVoiceInCmd("не %s;", node->val);
        
        PrintDiffMaskCriteria(tree, node->right, mask1, mask2, h + 1, obj1, obj2);
    }
}

void GuessObject(Tree* tree)
{
    PrintWithVoiceInCmd("Введите объект, определение которого ты хочешь узнать\n");\
    char object[MAX_CRIT_SIZE] = "";
    
    GetLine(stdin, object, MAX_CRIT_SIZE);
    #ifdef DEBUG
        printf("obj = <%s>\n", object);
    #endif

    long long mask =  GetMaskObjCritetia(tree, tree->root, object, 0);

    if (mask == 0)
        PrintWithVoiceInCmd("Это не постижимо! Объект не найден в моей базе! Ты жулик!\n");
    else
    {   
        PrintWithVoiceInCmd("%s - это", object);
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
        PrintWithVoiceInCmd(" не %s;", node->val);
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

static void GetLine(FILE* fp, char string[], int num)
{
    char* new_line_symb = nullptr;
    do 
    {
        fgets(string, num, fp);
        new_line_symb = strchr(string, '\n');
        if (new_line_symb != nullptr)
            *new_line_symb = '\0';

        #ifdef DEBUG
            int len = strlen(string);
            for(int i = 0; i < len; i++)
                printf("%d ", string[i]);
        #endif
    }
    while(new_line_symb == nullptr || strlen(string) == 0);
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

bool Akinate(Tree* tree, Node* node)
{
    PrintWithVoiceInCmd("%s?\n", node->val);

    char answer[MAX_CRIT_SIZE] = "";
    while(true)
    {
        GetLine(stdin, answer, MAX_CRIT_SIZE);

        #ifdef DEBUG
            printf("ans = <%s>\n", answer);
        #endif

        if (StrCmpDifferenCases(answer, "да") == 0)
        {
            if (node->left == nullptr)
            {
                PrintWithVoiceInCmd("Это %s. Это было легко!!!\n", node->val);
                return true;
            }

            return Akinate(tree, node->left);
        }
        if (StrCmpDifferenCases(answer, "нет") == 0)
        {
            if (node->right == nullptr)
            {
                PrintWithVoiceInCmd("Наконец-то достойный соперник! Наше стражение стало легендарным!\n"
                       "Кого ты загадал?\n");

                AddNewObjectAndCrit(tree, node);

                PrintWithVoiceInCmd("Теперь я стал сильнее! В этот раз тебе не удасться победить!!!\n");
                return true;
            }
            else
            {
                return Akinate(tree, node->right);
            }
        }
        if (StrCmpDifferenCases(answer, "выход") == 0)
            return false;
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

void RunAkinator(Tree* tree)
{
    while(true)
    {
        PrintWithVoiceInCmd("Режимы:\n"
               "-1 - Выход из программы\n" 
               "0  - Угадать\n"
               "1  - Дать определение\n"
               "2  - Сравнение\n"
               "3  - Графический дамп\n");

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
                GuessObject(tree);
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
        
        PrintWithVoiceInCmd("Начнём сначала!!!\n");
    }
}
