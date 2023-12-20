#ifndef PUNCTUATION_H_
#define PUNCTUATION_H_

#include "tokenCommon.h"

enum PunctiationType
{
    #define DEF_PUNCTUATIONS(type, name) PUNC_TYPE_ ## type,
        #include "punctuations_codegen.inc"
    #undef  DEF_PUNCTUATIONS
};

struct Punctuation
{
    TokenInfo info;
    PunctiationType type;
};

const Punctuation PUNCTUATIONS[] = 
{
    #define DEF_PUNCTUATIONS(type, name)\
        {{name, sizeof(name) - 1}, PUNC_TYPE_ ## type},

        #include "punctuations_codegen.inc"

    #undef  DEF_PUNCTUATIONS
};

const int N_PUNCTUATIONS = sizeof(PUNCTUATIONS) / sizeof(PUNCTUATIONS[0]);

#endif // PUNCTUATION_H_