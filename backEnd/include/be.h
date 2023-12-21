#ifndef BACK_END_H_
#define BACK_END_H_

#include <stdio.h>

#include "../../common/binaryTree/tree.h"
#include "../include/be_cfg.h"
#include "../../common/identifiers.h"

enum BackEndError
{
    #define DEF_BE_ERR(err, msg) BE_ERR_ ## err,

        #include "be_errs_codegen.inc"

    #undef  DEF_BE_ERR
};


struct beNameTableUnit
{
    const char* name;
    int          len;
    int ramPlacement;

    IdentifierType type;
};

struct BackEnd
{
    int labelIndex;
    int  varFreeIndex;
    bool isVarRamBusy[BE_VAR_RAM_SIZE];
    int freeReturnValueIndex;
    Stack NameTableStack;
    BackEndError error;
};

const char* beGetErrorMsg(BackEndError err);

// so sorry------+
//               V
void translateToAss      (BackEnd* be, TreeNode* node, FILE* outFile);
void translateGlobalVars (BackEnd* be, TreeNode* node, FILE* outFile);
void translationBeginning(                             FILE* outFile);

void backEndCtor(BackEnd* be);

void backEndSetLogFile(FILE* file);

#endif // BACK_END_H_