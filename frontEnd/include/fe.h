#ifndef FRONT_END_H_
#define FRONT_END_H_

#include "../../libs/memAllocations/memAllocations.h"
#include "../../libs/logs/logs.h"
#include "../../common/binaryTree/tree.h"
#include "../../common/tokenCommon.h"
#include "fe_tokenTree_cfg.h"
#include "fe_syntaxErrors.h"

#include <stdio.h>

struct OperatorsParent
{
    TokenInfo info;
    int type;
};

struct NameTableUnit
{
    const char* name;
    const int   len;
    Token       token;
};

enum FrontEndError
{
    #define DEF_FE_ERR(err, msg) FE_ERR_ ## err,

        #include "fe_errs_codegen.inc"

    #undef  DEF_FE_ERR
};

struct FrontEnd
{
    const char* fileName;
    Stack syntaxErrors;
    Stack nameTable;
    DynArr identifiers;
    DynArr consts;
    FrontEndError error;
};

const char* feGetErrorMsg(FrontEndError err);

void frontEndCtor(FrontEnd* fe, const char* fileName);
void frontEndDtor(FrontEnd* fe);

void frontEndSetLogFile(FILE* file);

Stack parseStrToTokens(FrontEnd* fe, const char* str);

const char* getTokenStr(Token* token, int* size);
int         getTokenID (Token* token);

Tree parseTokensToSyntaxTree(FrontEnd* fe, Stack* tokensStk);

void printSyntaxErrors(const FrontEnd* fe, const Stack* tokensStk);

const NameTableUnit* getNameTableUnitByName(FrontEnd* fe, const char* str);

#define FE_LOG_SET_ERROR(err)                                                   \
    do {                                                                        \
        fe->error = err;                                                        \
        LOGF_ERR(logFile, "%s\n", feGetErrorMsg(err));                          \
    } while(0)

#define FE_LOG_FUNC_START()                                                     \
    do {                                                                        \
        LOG_FUNC_START(logFile);                                                \
    } while(0)

#define FE_LOG_FUNC_END()                                                       \
    do {                                                                        \
        LOG_FUNC_END(logFile);                                                  \
    } while(0)

#define FE_LOGF_COLOR(color, ...)                                               \
    do {                                                                        \
        LOGF_COLOR(logFile, color, __VA_ARGS__);                                \
    } while(0)

#endif // FRONT_END_H_
