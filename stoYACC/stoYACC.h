#ifndef STOYACC_H_
#define STOYACC_H_

#include <stdio.h>

enum StoyaccError
{
        #define DEF_STOYACC_ERR(err, msg) STOYACC_ERR_ ## err,

            #include "stoYACC_errs_codegen.inc"

        #undef  DEF_STOYACC_ERR
};

struct Stoyacc
{
    
};

const char*  stoyaccGetErrorMsg(StoyaccError err);
void         stoyaccSetLogFile (FILE* file);

StoyaccError stoyaccGenCompiler(FILE* inFile, FILE* outFile, FILE* headerFile);

#endif // STOYACC_H_
