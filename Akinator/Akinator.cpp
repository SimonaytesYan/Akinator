#include "Akinator.h"
#include <stdio.h>

#define DEBUG

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

    DFS_f pre_function = [](Node* node, void* fp)
                       {
                            fprintf((FILE*)fp, " { ");
                            fprintf((FILE*)fp, node->val);
                       };
    DFS_f post_function = [](Node* node, void* fp)
                        {
                            fprintf((FILE*)fp, " } \n");
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
    {};
    ungetc(c, fp);

    int i = 0;
    while ((c = getc(fp)) != '{' && c != EOF && c != '}')
    {
        node->val[i] = c;
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
            scanf("%d", &operation_mode);

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
            Akinate(&tree, tree.root);
            break;
            
        case 2:
            Akinate(&tree, tree.root);
            break;
            
        case 3:
            GraphicDump(&tree);
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

bool Akinate(Tree* tree, Node* node)
{
    printf("%s?\n", node->val);

    char answer[20] = "";
    while(true)
    {
        fflush(stdin);
        scanf("%s", answer);

        #ifdef DEВUG
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

                char new_obj[MAX_CRIT_SIZE] = "";
                fflush(stdin);
                scanf("%s", new_obj);
                
                printf("Чем %s отличается от %s?\n", new_obj, node->val);

                char new_criteria[MAX_CRIT_SIZE] = "";
                fflush(stdin);
                scanf("%s", new_criteria);
                
                #ifdef DEВUG
                    printf("new_obj      = <%s>\n", new_obj);
                    printf("new_criteria = <%s>\n", new_criteria);
                #endif

                TreeInsertLeafLeft (tree, new_obj,   node);
                TreeInsertLeafRight(tree, node->val, node);
                
                strcpy(node->val, new_criteria);

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