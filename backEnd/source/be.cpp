#include "../include/be.h"

#include <assert.h>
#include "../../common/operations.h"
#include "../../common/identifiers.h"
#include "../../common/punctuations.h"
#include "../../common/numbers.h"
#include "../../common/keyWords.h"
#include "../../common/tokenCommon.h"
#include "../../frontEnd/include/fe.h"

static FILE* logFile = nullptr;

const char* beGetErrorMsg(BackEndError err)
{
    #define DEF_BE_ERR(err, msg)                                             \
        case BE_ERR_ ## err:                                                 \
            return msg;

    switch (err)
    {
        #include "../include/be_errs_codegen.inc"
        default:
            return "No such error was found";
    }
    #undef DEF_DIF_ERR
}

void backEndCtor(BackEnd* be)
{
    be->varFreeIndex = BE_VAR_RAM_START_POS;
    be->freeReturnValueIndex = BE_VAR_RET_START_POS;    
    for (int i = 0; i < BE_VAR_RAM_SIZE; i++)
    {
        be->isVarRamBusy[i] = false;
    }

    if (stackCtor(&be->NameTableStack, sizeof(beNameTableUnit)))
    {
        be->error = BE_ERR_MEM_STACK;
        return;
    }
}

void backEndSetLogFile(FILE* file)
{
    logFile = file;
}

static int getFreeVarIndex(BackEnd* be)
{
    assert(be);

    if (be->varFreeIndex < BE_VAR_RAM_SIZE)
    {
        if (be->isVarRamBusy[be->varFreeIndex] == false) // FIXME COPYPASTE
        {
            be->isVarRamBusy[be->varFreeIndex] =  true;
            return BE_VAR_RAM_START_POS + be->varFreeIndex++;
        }
    }
    else
    {
        be->varFreeIndex = 0;
    }

    for (int i = 0; i < BE_VAR_RAM_SIZE; i++)
    {
        if (be->isVarRamBusy[be->varFreeIndex] == false)
        {
            be->varFreeIndex = i;
            be->isVarRamBusy[be->varFreeIndex]  = true;
            return BE_VAR_RAM_START_POS + be->varFreeIndex++;
        }
    }
    return -1;
} 


static beNameTableUnit* getNameTableUnitByName(Stack* stk, const char* name)
{
    for (unsigned int i = 0; i < stk->size; i++)
    {
        beNameTableUnit curUnit = ((beNameTableUnit*)(stk->data))[i];
        if (strncmp(curUnit.name, name, curUnit.len) == 0) // FIXME add check by ptr value
        {
            return &((beNameTableUnit*)(stk->data))[i];
        }
    }
    return nullptr;
}


#define   operationToken(treeNode) (*(const   Operation*)(treeNode->data.structPtr))
#define     keyWordToken(treeNode) (*(const     KeyWord*)(treeNode->data.structPtr))
#define      numberToken(treeNode) (*(const      Number*)(treeNode->data.structPtr))
#define  identifierToken(treeNode) (*(const  Identifier*)(treeNode->data.structPtr))
#define punctuationToken(treeNode) (*(const Punctuation*)(treeNode->data.structPtr))

#define index (be->labelIndex)
#define printCmd(...) fprintf(outFile, "        " __VA_ARGS__ ) // FIXME cpu can't recognize tabs ????
#define printCom(...) fprintf(outFile, __VA_ARGS__)

static void translateExpression     (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateOperation      (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateKeyWord        (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateEvaluation     (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateVarDeclaration (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateFuncDeclaration(BackEnd* be, TreeNode* node, FILE* outFile);
static void translateIf             (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateWhile          (BackEnd* be, TreeNode* node, FILE* outFile);
static void translateReturn         (BackEnd* be, TreeNode* node, FILE* outFile);


void translationBeginning(FILE* outFile)
{
    printCmd("push 200\n");
    printCmd("pop rdx ; for recursion\n\n");

    printCmd("call main\n");
    printCmd("push [100]\n");
    printCmd("out\n");
    printCmd("HLT\n\n");
}


void translateGlobalVars(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(outFile);

    if (!node)
        return;
    if (node->leftBranch->leftBranch->data.type == TOKEN_OPERATION)
    {
        translateVarDeclaration(be, node->leftBranch->leftBranch, outFile);
    }
    translateGlobalVars(be, node->rightBranch, outFile);
}


void translateToAss(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(outFile);

    if (!node)
        return;

    switch (node->data.type)
    {
        case TOKEN_KEY_WORD:
            translateKeyWord(be, node, outFile);
            break;
        case TOKEN_OPERATION:
            translateOperation(be, node, outFile);
            break;
        case TOKEN_PUNCTUATION:
            if (punctuationToken(node).type == PUNC_TYPE_SEMICOLON)
            {
                translateToAss(be, node-> leftBranch, outFile);
                translateToAss(be, node->rightBranch, outFile);
            }
            break;
    }
}


static void translateOperation(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(outFile);

    if (operationToken(node).type == OPER_TYPE_ASGN)
    {
        translateEvaluation(be, node, outFile);
    }
}


static void translateKeyWord(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(outFile);

    if (keyWordToken(node).type == KEY_WORD_INT)
    {
        if (node->leftBranch->data.type == TOKEN_OPERATION)
        {
            translateVarDeclaration(be, node->leftBranch, outFile);
        }
        if (node->leftBranch->data.type == TOKEN_IDENTIFIER)
        {
            translateFuncDeclaration(be, node->leftBranch, outFile);
        }
    }
    else if (keyWordToken(node).type == KEY_WORD_IF)
    {
        translateIf(be, node, outFile);
    }
    else if (keyWordToken(node).type == KEY_WORD_WHILE)
    {
        translateWhile(be, node, outFile);
    }
    else if (keyWordToken(node).type == KEY_WORD_RETURN)
    {
        translateReturn(be, node, outFile);
    }
}


static void translateEvaluation(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(outFile);

    assert(operationToken(node).type == OPER_TYPE_ASGN);

    translateExpression(be, node->leftBranch, outFile);

    Identifier var = identifierToken(node->rightBranch);

    int ram = getNameTableUnitByName(&be->NameTableStack, var.info.name)->ramPlacement;

    printCmd("pop [%d] ; variable %.*s\n", ram, var.info.len, var.info.name);
}


static void translateExpression(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(outFile);

    if (node == nullptr)
        return;

    int logLen = 0; 
    const char* logTokenName = getTokenStr(&node->data, &logLen);
    LOGF_COLOR(logFile, green, "function started with %.*s\n", logLen, logTokenName);

    translateExpression(be, node-> leftBranch, outFile);
    translateExpression(be, node->rightBranch, outFile);
    index++;
    if (node->data.type == TOKEN_OPERATION)
    {
        switch (operationToken(node).type)
        {
            #define DEF_EXP(type, ...)                                              \
                case OPER_TYPE_ ## type:                                            \
                    printCmd(__VA_ARGS__);                                             \
                    printCmd("\n");                                                    \
                    break;

            #include "../include/be_expressionTranslator_codegen.inc"
            #undef  DEF_EXP
        }
    }
    else if (node->data.type == TOKEN_NUMBER)
    {
        printCmd("push %lg", numberToken(node).value); 
    }
    else if (node->data.type == TOKEN_IDENTIFIER)
    {
        Identifier iden = identifierToken(node);
        beNameTableUnit* unit = getNameTableUnitByName(&be->NameTableStack, iden.info.name);

        if (unit->type == IDENTIFIER_VAR)
        {
            printCmd("push [%d] ; variable %.*s\n", unit->ramPlacement,
                                              iden.info.len,
                                              iden.info.name);
        }
        else if (unit->type == IDENTIFIER_FUNC)
        {
            printCmd("call %.*s\n", iden.info.len,
                                 iden.info.name);
            printCmd("push rax\n");
        }
    }
    else
    {
        assert(0);
    }
    fputc('\n', outFile);

}

static void translateVarDeclaration(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(node->rightBranch);
    assert(node->leftBranch);
    assert(outFile);

    beNameTableUnit* unitPtr = getNameTableUnitByName(&be->NameTableStack,
                                                      identifierToken(node->rightBranch).info.name);
    if (unitPtr != nullptr)
        return;

    translateExpression(be, node->leftBranch, outFile);

    beNameTableUnit unit = 
    {
        .name = identifierToken(node->rightBranch).info.name,
        .len  = identifierToken(node->rightBranch).info.len, 
        .ramPlacement = getFreeVarIndex(be),
        .type = IDENTIFIER_VAR,
    };

    if (stackPush(&be->NameTableStack, &unit))
    {
        be->error = BE_ERR_MEM_STACK;
        return;
    }

    printCmd("pop [%d] ; variable %.*s\n", unit.ramPlacement, unit.len, unit.name);
}


static void translateFuncDeclaration(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(node->leftBranch);
    assert(outFile);

    beNameTableUnit unit = 
    {
        .name = identifierToken(node).info.name,
        .len =  identifierToken(node).info.len,
        .ramPlacement = -1,
        .type = IDENTIFIER_FUNC,
    };

    printCom("%.*s:\n\n", unit.len, unit.name);

    printCmd("pop [rdx]\n");
    printCmd("push rdx + 1\n");
    printCmd("pop rdx\n");

    if (stackPush(&be->NameTableStack, &unit))
    {
        be->error = BE_ERR_MEM_STACK;
        return;
    }

    translateToAss(be, node->leftBranch, outFile);
}


static void translateIf(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(node->leftBranch);
    assert(outFile);

    printCom("; if statement\n");

    translateExpression(be, node->leftBranch, outFile);

    be->labelIndex++;

    int curLabelIndex = be->labelIndex;
    printCmd("push 0\n");
    printCmd("jne IF_JMP_%d\n", curLabelIndex); 

    printCmd("jump IF_END_%d\n", curLabelIndex); 

    printCom("IF_JMP_%d:\n", curLabelIndex); 

    translateToAss(be, node->rightBranch, outFile);

    printCom("IF_END_%d:\n", curLabelIndex); 
}


static void translateWhile(BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(node->leftBranch);
    assert(outFile);

    be->labelIndex++;
    int curLabelIndex = be->labelIndex;

    printCom("\n; while statement\n\n");

    printCom("LOOP_START_%d:\n", curLabelIndex);

    printCmd("push 0\n");

    printCom("; condition start:\n");

    translateExpression(be, node->leftBranch, outFile);

    printCom("; condition end.\n");

    printCmd("je LOOP_END_%d\n", curLabelIndex);

    translateToAss(be, node->rightBranch, outFile);

    printCmd("jump LOOP_START_%d\n", curLabelIndex);

    printCom("LOOP_END_%d:\n", curLabelIndex);
}


static void translateReturn (BackEnd* be, TreeNode* node, FILE* outFile)
{
    LOG_FUNC_START(logFile);
    assert(be);
    assert(node);
    assert(node->leftBranch);
    assert(outFile);

    printCom("; Return statement\n");    
    translateExpression(be, node->leftBranch, outFile);
    printCmd("pop rax\n");

    printCom("; Get the ret value from the stack\n");
    printCmd("push rdx\n");
    printCmd("push 1\n");
    printCmd("sub\n");
    printCmd("pop rdx\n");
    printCmd("push [rdx]\n");
    printCmd("ret\n");
}