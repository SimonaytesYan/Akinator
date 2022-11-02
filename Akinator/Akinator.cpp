#include <stdio.h>

#include "Akinator.h"

#include <unistd.h>

#ifdef _WIN32
#include "Libs/TXLib.h"
#endif

#define DEBUG

static void GetLine(FILE* fp, char string[], int num);

int StrCmpDifferenCases(const char* s1, const char* s2)
{
    #ifdef _WIN32
        return stricmp(s1, s2);
    #else
        return strcasecmp(s1, s2);
    #endif
}

void InitAkinator(Tree* tree)
{
    OpenHtmlLogFile("Akinator.html");
    
    *tree = {};
    TreeCtor(tree);
    GetTreeFromFile(tree, DEFAULT_TREE_NAME);

    #ifdef DEBUG
        printf("%p\n", tree->root);
    #endif
}

void DeInitAkinator(Tree* tree)
{
    SaveTreeInFile(tree, DEFAULT_TREE_NAME);

    TreeDtor(tree);

    CloseHtmlLogFile();
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

bool GetObjCriteria(Tree* tree, Node* node, char object[MAX_CRIT_SIZE])
{
    if (node == nullptr)
        return false;
    
    if (node->left == nullptr && node->right == nullptr)
    {
        #ifdef DEBUG
            printf("leaf    = <%s>\n", node->val);
            int cmp_res = StrCmpDifferenCases(object, node->val);
            printf("cmp_res = %d\n", cmp_res); 
        #endif

        if (StrCmpDifferenCases(object, node->val) == 0)
        {
            printf("%s - ", node->val);
            return true;
        }
    }

    bool result = false;

    if (GetObjCriteria(tree, node->left, object))
    {
        printf("%s", node->val);

        result = true;
    }
    else if (GetObjCriteria(tree, node->right, object))
    {
        printf("не %s", node->val);
        
        result = true;
    }

    if (result)
    {
        if (node == tree->root)
            printf("\n");
        else
            printf(", ");
    }

    return result;
}

void RunAkinator()
{
    Tree tree = {};
    InitAkinator(&tree);
    
    while(true)
    {
        printf("Режимы:\n"
               "-1 - Выход из программы\n" 
               "0  - Угадать\n"
               "1  - Дать определение\n"
               "2  - Сравнение\n"
               "3  - Графический дамп\n"
               "В каком режиме запустить программу?\n");

        int operation_mode = -2;

        while (operation_mode < -1 || operation_mode > 3)
        {
            scanf("%d", &operation_mode);
            printf("Oper_mode = %d\n", operation_mode);
        }

        bool end_program = false;
        switch (operation_mode)
        {
            case -1:
                end_program = true;
                break;
            case 0:
            {
                Akinate(&tree, tree.root);
                break;
            }

            case 1:
            {
                printf("Введите объект, определение которого ты хочешь узнать\n");
                char object[MAX_CRIT_SIZE] = "";
                
                GetLine(stdin, object, MAX_CRIT_SIZE);

                #ifdef DEBUG
                    printf("obj = <%s>\n", object);
                #endif

                if (!GetObjCriteria(&tree, tree.root, object))
                    printf("Это не постижимо! Объект не найден в моей базе! Ты жулик!\n");
                break;
            }
            
            case 2:
            {
                Akinate(&tree, tree.root);
                break;
            }
            
            case 3:
            {
                GraphicDump(&tree);
                char graphic_dump_file[70] = "";

                #ifdef _WIN32
                    sprintf(graphic_dump_file, ".\\GraphicDumps\\Dump%d.png", GRAPHIC_DUMP_CNT - 1);
                #else
                    sprintf(graphic_dump_file, "xdg-open GraphicDumps/Dump%d.png", GRAPHIC_DUMP_CNT - 1);
                #endif

                system(graphic_dump_file);

                break;
            }
            default:
                printf("Интересный у тебя operation mode, конечно...\n");
                break;
        }

        if (end_program)
            break;
        
        printf("Начнём сначала!!!\n");
    }

    DeInitAkinator(&tree);
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
    fflush(fp);

    fgets(string, num, fp);

    char* new_line = strchr(string, '\n');
    *new_line = '\0';
}

void AddNewObjectAndCrit(Tree* tree, Node* node)
{
    char new_obj[MAX_CRIT_SIZE] = "";
    GetLine(stdin, new_obj, MAX_CRIT_SIZE);
                
    printf("Чем %s отличается от %s?\n", new_obj, node->val);

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
    printf("%s?\n", node->val);

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
                printf("Это %s. Это было легко!!!\n", node->val);
                return true;
            }

            return Akinate(tree, node->left);
        }
        if (StrCmpDifferenCases(answer, "нет") == 0)
        {
            if (node->right == nullptr)
            {
                printf("Наконец-то достойный соперник! Наше стражение стало легендарным!\n"
                       "Кого ты загадал?\n");

                AddNewObjectAndCrit(tree, node);

                printf("Теперь я стал сильнее! В этот раз тебе не удасться победить!!!\n");
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