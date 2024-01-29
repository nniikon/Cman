#include "../include/fe.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "../../common/binaryTree/tree_graphDump.h"
#include "../../libs/ansiColors/ansiColors.h"
#include "../../common/operations.h"
#include "../../common/keyWords.h"
#include "../../common/identifiers.h"
#include "../../common/numbers.h"
#include "../../common/punctuations.h"

static FILE* logFile = nullptr;

static void pushToNameTable          (FrontEnd* fe, const void* opr, int nOpr,
                                                size_t oprSize, TokenType type);
static void dumpNameTable            (FrontEnd* fe);
static void dumpTokens(Stack* tokenStk);

const char* feGetErrorMsg(FrontEndError err)
{
    #define DEF_FE_ERR(err, msg)                                             \
        case FE_ERR_ ## err:                                                 \
            return msg;

    switch (err)
    {
        #include "../include/fe_errs_codegen.inc"
        default:
            return "No such error was found";
    }
    #undef DEF_DIF_ERR
}


int getTokenID(Token* token)
{
    switch (token->type)
    {
        case TOKEN_NUMBER:
        case TOKEN_IDENTIFIER:
            return -1;
        case TOKEN_WHITE_SPACE:
            return (int) token->type;
        case TOKEN_KEY_WORD:
        case TOKEN_OPERATION:
        case TOKEN_PUNCTUATION:
            return ((const OperatorsParent*)(token->structPtr))->type | token->type;
        
        default:
            assert(0);
            return 0;
    }
}


const char* getTokenStr(Token* token, int* size)
{
    assert(token);
    assert(size);

    if (!token || !token->structPtr)
        return nullptr;

    switch (token->type)
    {
        case TOKEN_OPERATION:
        case TOKEN_NUMBER:
        case TOKEN_IDENTIFIER:
        case TOKEN_KEY_WORD:
        case TOKEN_PUNCTUATION:
            *size = ((const OperatorsParent*)token->structPtr)->info.len;
            return  ((const OperatorsParent*)token->structPtr)->info.name;

        case TOKEN_WHITE_SPACE:
            *size = 2;
            return "WS";
        default:
            *size = 0;
            return nullptr;
    }
}


static void dumpTokens(Stack* tokenStk)
{
    FE_LOG_FUNC_START();

    assert(tokenStk);
    if (!tokenStk)
        return;

    Token* tokens = (Token*) tokenStk->data;

    LOG_START_COLOR(logFile, white);
    fprintf(logFile, "\nDump Tokens:\n");

    for (unsigned i = 0; i < tokenStk->size; i++)
    {
        int size = 0;
        getTokenStr(tokens + i, &size);
        fprintf(logFile, "%-*u", size + 3, i); 
    }
    fprintf(logFile, "\n");

    for (unsigned i = 0; i < tokenStk->size; i++)
    {
        int size = 0;
        const char* tokenName = getTokenStr(tokens + i, &size);
        fprintf(logFile, "%.*s   ", size, tokenName); 
    }

    fprintf(logFile, "\n\n");
    LOG_END(logFile);

    FE_LOG_FUNC_END();
}


static void dumpNameTable(FrontEnd* fe)
{
    FE_LOG_FUNC_START();

    assert(fe);
    if (!fe)
    {
        FE_LOG_SET_ERROR(FE_ERR_NULLPTR_PASSED);
        return;
    }

    NameTableUnit* nameTable = (NameTableUnit*) fe->nameTable.data;

    LOG_START_COLOR(logFile, white);
    fprintf(logFile, "\nName Table:\n");

    for (unsigned int i = 0; i < fe->nameTable.size; i++)
    {
        fprintf(logFile, "%-*u", nameTable[i].len + 2, i);
    }
    fprintf(logFile, "\n");

    for (unsigned int i = 0; i < fe->nameTable.size; i++)
    {
        fprintf(logFile, "%.*s  ", nameTable[i].len,
                                   nameTable[i].name);
    }

    fprintf(logFile, "\n\n");
    LOG_END(logFile);

    FE_LOG_FUNC_END();
}


static void pushToNameTable(FrontEnd* fe, const void* opr, int nOpr, size_t oprSize, TokenType type)
{
    FE_LOG_FUNC_START();

    assert(opr);
    assert(fe);
    if (!fe || !opr)
    {
        FE_LOG_SET_ERROR(FE_ERR_NULLPTR_PASSED);
        return;
    }

    for (int i = 0; i < nOpr; i++)
    {
        const OperatorsParent *curOpr = (const OperatorsParent*)((const char*)opr + i * oprSize);
        NameTableUnit unit = 
        {
            .name  = curOpr->info.name,
            .len   = curOpr->info.len,
            .token = 
            {
                .type = type,
                .structPtr = (const void*)(curOpr),
            }
        };

        if (stackPush(&fe->nameTable, &unit))
        {
            FE_LOG_SET_ERROR(FE_ERR_MEM_STACK);
            return;
        }
        FE_LOGF_COLOR(blue, "Put keyword %-8.*s into the name table\n",
                               curOpr->info.len, curOpr->info.name);
    }

    dumpNameTable(fe);
    FE_LOG_FUNC_END();
}


void frontEndSetLogFile(FILE* file)
{
    logFile = file;
}


// FIXME: add goto 
void frontEndCtor(FrontEnd* fe, const char* fileName)
{
    FE_LOG_FUNC_START();

    assert(fe);
    if (!fe)
    {
        FE_LOG_SET_ERROR(FE_ERR_NULLPTR_PASSED);
        return;
    }

    if (stackCtor(&fe->nameTable, sizeof(NameTableUnit)))
    {
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK); 
        return;
    }

    if (dynArrCtor(&fe->identifiers, sizeof(Identifier)))
    {
        stackDtor(&fe->nameTable);
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);
        return;
    }

    if (dynArrCtor(&fe->consts, sizeof(Number)))
    {
        dynArrDtor(&fe->identifiers);
        stackDtor(&fe->nameTable);
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);
        return;
    }

    if (stackCtor(&fe->syntaxErrors, sizeof(SyntaxError)))
    {
        dynArrDtor(&fe->consts);
        dynArrDtor(&fe->identifiers);
        stackDtor (&fe->nameTable);
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK); 
        return;
    }

    fe->fileName = fileName;

    pushToNameTable(fe, OPERATIONS, N_OPERATIONS, sizeof(Operation), TOKEN_OPERATION);
    if (fe->error)
    {
        stackDtor (&fe->syntaxErrors);
        dynArrDtor(&fe->consts);
        dynArrDtor(&fe->identifiers);
        stackDtor (&fe->nameTable);
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK); 
        return;
    }

    pushToNameTable(fe, KEY_WORDS, N_KEY_WORDS, sizeof(KeyWord), TOKEN_KEY_WORD);
    if (fe->error)
    {
        stackDtor (&fe->syntaxErrors);
        dynArrDtor(&fe->consts);
        dynArrDtor(&fe->identifiers);
        stackDtor (&fe->nameTable);
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK); 
        return;
    }

    pushToNameTable(fe, PUNCTUATIONS, N_PUNCTUATIONS, sizeof(Punctuation), TOKEN_PUNCTUATION);
    if (fe->error)
    {
        stackDtor (&fe->syntaxErrors);
        dynArrDtor(&fe->consts);
        dynArrDtor(&fe->identifiers);
        stackDtor (&fe->nameTable);
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK); 
        return;
    }

    FE_LOG_FUNC_END();
}


const NameTableUnit* getNameTableUnitByName(FrontEnd* fe, const char* str)
{
    assert(str);
    assert(fe);

    FE_LOGF_COLOR(white, "Trying to get a name from the name table\n");
    NameTableUnit* units = (NameTableUnit*) fe->nameTable.data;

    for (unsigned int i = 0; i < fe->nameTable.size; i++)
    {
        if (strncasecmp(str, units[i].name, units[i].len) == 0)
        {
            FE_LOGF_COLOR(SpringGreen, "Success, found %.*s\n", units[i].len, 
                                                                units[i].name);

            // If the identifier string is still going.
            if (isalnum(str[units[i].len]) && units[i].token.type == TOKEN_IDENTIFIER)
                continue;

            return units + i;
        }
    }
    return nullptr;
}


void frontEndDtor(FrontEnd* fe)
{
    FE_LOG_FUNC_START();

    assert(fe);
    if (!fe)
    {
        FE_LOG_SET_ERROR(FE_ERR_NULLPTR_PASSED);
        return;
    }

    if (stackDtor(&fe->nameTable))
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK); 
    
    if (dynArrDtor(&fe->identifiers))
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);
    
    if (dynArrDtor(&fe->consts))
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);

    if (stackDtor(&fe->syntaxErrors))
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK);

    FE_LOG_FUNC_END();
}


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// LEXER /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


static bool tryNameToken       (FrontEnd* fe, const char* str, int* size, Token* token);
static bool tryNumberToken     (FrontEnd* fe, const char* str, int* size, Token* token);
static bool tryIndentifierToken(FrontEnd* fe, const char* str, int* size, Token* token);
static bool tryWhiteSpaceToken (FrontEnd* fe, const char* str, int* size, Token* token, int* nNewLines);

Stack parseStrToTokens(FrontEnd* fe, const char* str)
{
    FE_LOG_FUNC_START();

    assert(fe);
    assert(str);
    if (!fe || !str)
    {
        FE_LOG_SET_ERROR(FE_ERR_NULLPTR_PASSED);
        return {};
    }

    Stack tokensStk = {};
    if (stackCtor(&tokensStk, sizeof(Token)))
    {
        FE_LOG_SET_ERROR(FE_ERR_MEM_STACK);
        return {};
    }

    const char*  currentLine = str; 
    unsigned int currentLineLen = (unsigned int)(strchr(str, '\n') - str);
    unsigned int lineNumber  = 1;
    unsigned int currentChar = 0;


    while(str[0])
    {
        FE_LOGF_COLOR(orange, "lexer started with %s\n", str);

        Token newToken = {};
        int  tokenSize = 0;
        int nNewLines = 0;

        if (!tryWhiteSpaceToken (fe, str, &tokenSize, &newToken, &nNewLines) &&
            !tryNameToken       (fe, str, &tokenSize, &newToken) &&
            !tryIndentifierToken(fe, str, &tokenSize, &newToken) && 
            !tryNumberToken     (fe, str, &tokenSize, &newToken)    )
        {
            fprintf(stderr, "Syntax error!\n"); // FIXME: add syntax errors
        }

        if (fe->error)
            return {};

        str += tokenSize;

        if (nNewLines != 0)
        {
            lineNumber += nNewLines;
            currentLineLen = (unsigned int)(strchr(str, '\n') - str);
            currentLine = str;
            currentChar = 0;
        }
        else
        {
            currentChar += tokenSize;
        }

        newToken.posInfo.curLineLen = currentLineLen;
        newToken.posInfo.charNumber = currentChar;
        newToken.posInfo.lineNumber = lineNumber;
        newToken.posInfo.curLine    = currentLine;

        int stkErr = stackPush(&tokensStk, &newToken);
        if (stkErr)
        {
            FE_LOG_SET_ERROR(FE_ERR_MEM_STACK);
            stackDtor(&tokensStk);
            return {};
        }
    }
    dumpTokens(&tokensStk);

    FE_LOG_FUNC_END();
    return tokensStk;
}


static bool tryNameToken(FrontEnd* fe, const char* str, int* size, Token* token)
{
    assert(str);
    assert(size);
    assert(token);

    FE_LOGF_COLOR(white, "Trying to get a name from the name table\n");

    const NameTableUnit* unit = getNameTableUnitByName(fe, str);
    if (!unit)
        return false;

    *size = unit->len;
    *token = unit->token;
    return true;
}


static bool tryNumberToken(FrontEnd* fe, const char* str, int* size, Token* token)
{
    assert(str);
    assert(size);
    assert(token);

    FE_LOGF_COLOR(white, "Trying to get a number\n");

    if (!isdigit(str[0]) && str[0] != '-' && str[0] != '+')
        return false;

    Number* newConst = (Number*) dynArrCalloc(&fe->consts);
    if (!newConst)
    {
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);
        return false;
    }

    char* end = nullptr;
    double value = strtod(str, &end);

    *size = (int)(end - str);

    newConst->info.len  = *size;
    newConst->info.name = str;
    newConst->value = value;

    token->type   = TOKEN_NUMBER;
    token->structPtr = newConst;

    if (errno)
    {
        // FIXME: add syntax error handling
        fprintf(stderr, "syntax error\n");
        errno = 0;
    }

    FE_LOGF_COLOR(SpringGreen, "Success, found %lg\n", value);

    return true;
}


static bool tryWhiteSpaceToken(FrontEnd* fe, const char* str, int* size, Token* token, int* nNewLines)
{
    assert(str);
    assert(size);
    assert(token);

    FE_LOGF_COLOR(white, "Trying to get a white space\n");

    if (!isspace(str[0]))
        return false;

    *size = 0;
    *nNewLines = 0;
    while (isspace(str[*size]))
    {
        if (str[*size] == '\n')
        {
            (*nNewLines)++;
        }
        (*size)++;
    }
    
    token->type = TOKEN_WHITE_SPACE;

    FE_LOGF_COLOR(SpringGreen, "Skipped spaces with %d new lines\n", *nNewLines);

    return true;
}


static bool tryIndentifierToken(FrontEnd* fe, const char* str, int* size, Token* token)
{
    assert(str);
    assert(size);
    assert(token);

    FE_LOGF_COLOR(white, "Trying to get an identifier\n");

    if (!isalpha(str[0]))
        return false;

    *size = 1;
    while (isalnum(str[*size]))
        (*size)++;
    
    Identifier* newIdentifier = ((Identifier*) dynArrCalloc(&fe->identifiers));
    if (!newIdentifier)
    {
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);
        return false;
    }
    newIdentifier->info.name = str;
    newIdentifier->info.len  = *size;
    newIdentifier->nameTableID = fe->nameTable.size;

    FE_LOGF_COLOR(grey, "nameTableID: %u\n", newIdentifier->nameTableID);

    NameTableUnit unit = 
    {
        .name = str,
        .len  = *size,
        .token = 
        {
            .type = TOKEN_IDENTIFIER,
            .structPtr = newIdentifier,
        }
    };
    *token = unit.token;

    if (stackPush(&fe->nameTable, &unit))
    {
        FE_LOG_SET_ERROR(FE_ERR_MEM_DYN_ARR);
        return false;
    }

    FE_LOGF_COLOR(SpringGreen, "Successfully added a new unit: %.*s\n", *size, str);
    dumpNameTable(fe);

    return true;
}


///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// PARSER /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define ASSERT_ALL()                                                           \
    FE_LOG_FUNC_START();                                                       \
    FE_LOGF_COLOR(SpringGreen, "Current position: %u\n", *pos);                \
    assert(fe);                                                                \
    assert(tree);                                                              \
    assert(pos); 

#define tokenAt(pos)        (((const Token*)tokensStk->data)[pos])

#define keyWordToken(pos)     (*((const KeyWord*)    (tokenAt(pos)).structPtr))
#define identifierToken(pos)  (*((const Identifier*) (tokenAt(pos)).structPtr))
#define operationToken(pos)   (*((const Operation*)  (tokenAt(pos)).structPtr))
#define punctuationToken(pos) (*((const Punctuation*)(tokenAt(pos)).structPtr))

#define createIdentifierToken(pos) (((const NameTableUnit*)(fe->nameTable.data))[identifierToken(pos).nameTableID].token)
#define createSemicolonToken() {.type = TOKEN_PUNCTUATION, .structPtr = PUNCTUATIONS + (int)PUNC_TYPE_SEMICOLON}
#define createSemicolonNode() treeCreateNode(tree, NULL, NULL, NULL, createSemicolonToken())

#define pushSyntaxError(pos, errStr)                                           \
    do {                                                                       \
        SyntaxError syntaxError_ = {.tokenPos = pos, .str = errStr};           \
        if (stackPush(&fe->syntaxErrors, &syntaxError_))                       \
        {                                                                      \
            FE_LOG_SET_ERROR(FE_ERR_MEM_STACK);                                \
            return nullptr;                                                    \
        }                                                                      \
    } while(0)

#define isKeyword(word)     (tokenAt(*pos).type == TOKEN_KEY_WORD    &&     keyWordToken(*pos).type == (word))
#define isOperation(oper)   (tokenAt(*pos).type == TOKEN_OPERATION   &&   operationToken(*pos).type == (oper))
#define isPunctuation(punc) (tokenAt(*pos).type == TOKEN_PUNCTUATION && punctuationToken(*pos).type == (punc))

#define createNode(node)                                                       \
    (node) = treeCreateNode(tree, nullptr, nullptr,                            \
                                          nullptr, tokenAt(*pos));             \
    if (!node)                                                                 \
        FE_LOG_SET_ERROR(FE_ERR_TREE);                                         \


static TreeNode* getAll       (FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos);
static bool      getWhiteSpace(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos);
static TreeNode* getSpecifier (FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos);
static TreeNode* getIdentifier(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos);


static void printSyntaxError(const FrontEnd* fe, const Stack* tokensStk, SyntaxError* err)
{
    FE_LOG_FUNC_START();
    assert(fe);
    assert(err);

    fprintf(stdout, ANSI_BOLD "%s:%u:%u: " ANSI_COLOR_RESET, fe->fileName,
            tokenAt(err->tokenPos).posInfo.lineNumber, tokenAt(err->tokenPos).posInfo.charNumber);

    fprintf(stdout, ANSI_COLOR_RED "error: " ANSI_COLOR_RESET);
    fprintf(stdout, "%s\n", err->str);
    fprintf(stdout, "%6d | %.*s\n",  (int) tokenAt(err->tokenPos).posInfo.lineNumber,
                                     (int) tokenAt(err->tokenPos).posInfo.curLineLen,
                                           tokenAt(err->tokenPos).posInfo.curLine    );
    fprintf(stdout, "%7c|\n", ' ');

    FE_LOG_FUNC_END();
}


void printSyntaxErrors(const FrontEnd* fe, const Stack* tokensStk)
{
    FE_LOG_FUNC_START();
    assert(fe);

    for (int i = fe->syntaxErrors.size - 1; i >= 0; i--)
    {
        printSyntaxError(fe, tokensStk, (SyntaxError*)(fe->syntaxErrors.data) + i);
    }
    
    FE_LOG_FUNC_END();
}


Tree parseTokensToSyntaxTree(FrontEnd* fe, Stack* tokensStk)
{
    assert(fe);
    assert(tokensStk);

    if (!fe || !tokensStk)
    {
        FE_LOG_SET_ERROR(FE_ERR_NULLPTR_PASSED);
        return {};
    }

    Tree syntaxTree = {};
    if (treeCtor(&syntaxTree, logFile))
    {
        FE_LOG_SET_ERROR(FE_ERR_TREE);
        return {};
    }

    unsigned int pos = 0;
    syntaxTree.rootBranch = getAll(fe, &syntaxTree, tokensStk, &pos);

    treeGraphDump(&syntaxTree);

    return syntaxTree;
}

const char* getIdentifier_errStr = "expected an identifier";
static TreeNode* getIdentifier(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{
    ASSERT_ALL();

    if (tokenAt(*pos).type != TOKEN_IDENTIFIER)
    {
        // FIXME_ERR_HANDLING: ...
        LOGF_ERR(logFile, "Unable to recognize an identifier, current position: %u\n", *pos);
        return nullptr;
    }

    TreeNode* node = treeCreateNode(tree, nullptr, nullptr, nullptr, createIdentifierToken(*pos));
    if (!node)
        FE_LOG_SET_ERROR(FE_ERR_TREE);

    FE_LOGF_COLOR(gray, "Found identifier %.*s at position %u\n", identifierToken(*pos).info.len,
                                                                  identifierToken(*pos).info.name, *pos);
    (*pos)++;

    FE_LOG_FUNC_END();
    return node;
}

const char* getNumber_errStr = "expected a number";
static TreeNode* getNumber(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{
    ASSERT_ALL();

    if (tokenAt(*pos).type != TOKEN_NUMBER)
    {
        LOGF_ERR(logFile, "Unable to recognize a number, current position: %u\n", *pos);
        return nullptr;
    }

    FE_LOGF_COLOR(gray, "Found number %.*s at position %u\n", keyWordToken(*pos).info.len,
                                                              keyWordToken(*pos).info.name, *pos);

    TreeNode* node = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
    if (!node)
        FE_LOG_SET_ERROR(FE_ERR_TREE);
    (*pos)++;

    FE_LOG_FUNC_END();
    return node;
}

const char* getSpecifier_errStr = "expected a specifier";
static TreeNode* getSpecifier(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{
    ASSERT_ALL();

    if (!isKeyword(KEY_WORD_INT)) // FIXME: add isSpecifier function
    {
        LOGF_ERR(logFile, "Unable to recognize a specifier, current position: %u\n", *pos);
        return nullptr;
    }

    FE_LOGF_COLOR(gray, "Found specifier %.*s at position %u\n", keyWordToken(*pos).info.len,
                                                                 keyWordToken(*pos).info.name, *pos);

    TreeNode* node = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));
    if (!node)
        FE_LOG_SET_ERROR(FE_ERR_TREE);
    (*pos)++;

    FE_LOG_FUNC_END();
    return node;
}


static bool getWhiteSpace(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)
{
    ASSERT_ALL();

    if (tokenAt(*pos).type != TOKEN_WHITE_SPACE)
        return false;

    FE_LOGF_COLOR(gray, "Got a white space at position: %u\n", *pos);
    (*pos)++;

    FE_LOG_FUNC_END();
    return true;
}


#include "../../stoYACC_generated1.h"
#include "../../stoYACC_generated2.h"
