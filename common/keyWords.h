#ifndef KEY_WORDS_H_
#define KEY_WORDS_H_

#include "tokenCommon.h"

enum KeyWordType
{
    #define DEF_KEY_WORD(type, ...) KEY_WORD_ ## type,

        #include "keyWords_codegen.inc"

    #undef  DEF_KEY_WORD
};

struct KeyWord
{
    TokenInfo info;
    KeyWordType type;
};

const KeyWord KEY_WORDS[] = 
{
    #define DEF_KEY_WORD(type, name)\
        {{name, sizeof(name) - 1}, KEY_WORD_ ## type},

        #include "keyWords_codegen.inc"

    #undef  DEF_KEY_WORD
};

const int N_KEY_WORDS = sizeof(KEY_WORDS) / sizeof(KEY_WORDS[0]);

#endif // KEY_WORDS_H_