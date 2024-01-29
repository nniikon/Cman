#include "tree_saveLoad.h"

#include <assert.h>
#include <ctype.h>
#include "../../frontEnd/include/fe_tokenTree_cfg.h"
#include "../../libs/fileToBuffer/fileToBuffer.h"
#include "../identifiers.h"
#include "../numbers.h"
#include "../operations.h"
#include "../keyWords.h"
#include "../punctuations.h"
#include "../../frontEnd/include/fe.h"
#include "../../frontEnd/include/fe_tokenTree_cfg.h"

static FILE* logFile = NULL;

void treeSaveLoadSetLogFile(FILE* file)
{
    logFile = file;
}

static char* skipWhiteSpaces(char* str)
{
    assert(str);

    while (isspace(str[0]))
        str++;
    
    return str;
}


static const Token getTokenByType(int tokenType, int operType)
{
    const void* structPtr = NULL;
    switch (tokenType)
    {
        case TOKEN_OPERATION:   structPtr = (const void*)(OPERATIONS   + operType);
            break;
        case TOKEN_PUNCTUATION: structPtr = (const void*)(PUNCTUATIONS + operType);
            break;
        case TOKEN_KEY_WORD:    structPtr = (const void*)(KEY_WORDS    + operType);
            break;
        default:
            fprintf(stderr, "current token: %d\n", tokenType);
            assert(0);
    }
    Token token = {.type = (TokenType)tokenType, .structPtr = structPtr};
    return token;
}

static TreeNode* treeLoadNodeFromFile_recursive(char** str, Tree* tree, TreeError* err, FrontEnd* fe);


static bool treeLoadNodeGetBranch(char** str, Tree* tree, TreeError* err, FrontEnd* fe, TreeNode** branch)
{
    LOGF_COLOR(logFile, white, "The investigation is still ongoing\n");
    LOGF_COLOR(logFile, white, "The target is: %s\n", *str);
    if ((*str)[0] == '(') // FIXME COPYPASTE
    {
        LOGF_COLOR(logFile, purple, "Found a bracket!\n");
        *branch = treeLoadNodeFromFile_recursive(str, tree, err, fe);
        if (*branch == NULL)
            return false;
    }
    else
    {
        if (strncmp(*str, TREE_FILE_NIL_NAME, sizeof(TREE_FILE_NIL_NAME) - 1) == 0)
        {
            LOGF_COLOR(logFile, cyan, "Found a nil!\n");
            *branch = NULL;
            *str += sizeof(TREE_FILE_NIL_NAME) - 1;
            *str = skipWhiteSpaces(*str);
        }
        else
        {
            LOGF_ERR(logFile, "found error!\n");
            return false;
        }
    }
    return true;

}


static TreeNode* treeLoadNodeFromFile_recursive(char** str, Tree* tree, TreeError* err, FrontEnd* fe)
{
    assert(str);
    LOG_FUNC_START(logFile);
    LOGF_COLOR(logFile, white, "recursion started with %s\n", *str);

    if (*err)
    {
        LOGF_ERR(logFile, "found error!\n");
        return NULL;
    }

    if ((*str)[0] != '(')
    {
        LOGF_ERR(logFile, "found error!\n");
        return NULL;
    }

    (*str)++;
    *str = skipWhiteSpaces(*str);

    TreeNode* node = treeCreateEmptyNode(tree);
    if (!node)
    {
        *err = TREE_ERROR_BAD_MEM_ALLOC;
        return NULL;
    }

    if ((*str)[0] == '$')
    {
        LOGF_COLOR(logFile, green, "found a $, investigating further\n");
        (*str)++;
        if (isalpha((*str)[0]))
        {
            LOGF_COLOR(logFile, green, "found a word, wonder what it is\n");
            node->data.type = TOKEN_IDENTIFIER;

            Identifier* newIdentifier = (Identifier*) dynArrCalloc(&fe->identifiers);
            if (!newIdentifier)
            {
                *err = TREE_ERROR_BAD_MEM_ALLOC;
                return NULL;
            }

            char* end = strchr(*str, ' ');
            newIdentifier->info.name = *str; 
            newIdentifier->info.len  = (int)(end - *str);
            node->data.structPtr = (Identifier*) newIdentifier;

            *str = end;
        }
        else
        {
            LOGF_COLOR(logFile, green, "found a number, wonder what it is\n");
            node->data.type = TOKEN_NUMBER;

            Number* newNumber = (Number*) dynArrCalloc(&fe->consts);
            if (!newNumber)
            {
                *err = TREE_ERROR_BAD_MEM_ALLOC;
                return NULL;
            }

            char* end = NULL;
            double number = strtod(*str, &end);
            newNumber->info.name = *str;
            newNumber->info.len  = (int)(end - *str);
            newNumber->value     = number;
            node->data.structPtr = (Number*) newNumber;

            LOGF_COLOR(logFile, orange, "str: %.*s\n", newNumber->info.len,
                                                       newNumber->info.name);
            LOGF_COLOR(logFile, orange, "value: %lg\n", newNumber->value);

            *str = end;
        }
    }
    else
    {
        LOGF_COLOR(logFile, green, "We're dealing with something simple, not gonna take that much time, is it\n");
        char* end = NULL;
        int tokenID = (int) strtol(*str, &end, 10);
        int tokenType = tokenID & TOKEN_BITS;
        int operType  = tokenID &  OPER_BITS;

        LOGF_COLOR(logFile, green, "tokenType: %d, operType: %d\n", tokenType, operType); 

        node->data = getTokenByType(tokenType, operType);

        *str = end ;
    }

    *str = skipWhiteSpaces(*str);

    if (!treeLoadNodeGetBranch(str, tree, err, fe, &node->leftBranch ))
        return NULL;
    if (!treeLoadNodeGetBranch(str, tree, err, fe, &node->rightBranch))
        return NULL;

    if ((*str)[0] != ')')
    {
        LOGF_ERR(logFile, "found error!\n");
        return NULL;
    }

    (*str)++;
    *str = skipWhiteSpaces(*str);

    LOG_FUNC_END(logFile);
    return node;
}


Tree treeLoadFromFile(FILE* file, FrontEnd* fe, TreeError* err)
{
    Tree tree = {};
    *err = treeCtor(&tree, NULL);
    if (*err)
        return tree;

    char* str = (char*) ftbPutFileToBuffer(NULL, file);
    if (!str)
    {
        *err = TREE_ERROR_BAD_FREAD;
        treeDtor(&tree);
        return tree;
    }
    char* const oldStr = str;
    tree.rootBranch = treeLoadNodeFromFile_recursive(&str, &tree, err, fe);
    // FIXME
    return tree;
}
