#ifndef FE_TOKEN_TREE_CFG_H_
#define FE_TOKEN_TREE_CFG_H_

const int TOKEN_BITS = 0b1111'1111'0000'0000;
const int  OPER_BITS = 0b0000'0000'1111'1111;

enum TokenType
{
    TOKEN_OPERATION   = 1 << (8 + 1),
    TOKEN_IDENTIFIER  = 1 << (8 + 2),
    TOKEN_KEY_WORD    = 1 << (8 + 3),
    TOKEN_WHITE_SPACE = 1 << (8 + 4),
    TOKEN_PUNCTUATION = 1 << (8 + 5),
    TOKEN_NUMBER      = 1 << (8 + 6),
};

struct TokenPosInfo
{
    const char*  curLine;
    unsigned int curLineLen;
    unsigned int lineNumber;
    unsigned int charNumber;
};

struct Token
{
    TokenType type;
    const void* structPtr;
    TokenPosInfo posInfo;
};

typedef Token treeElem_t;

const char* getTokenTypeStr(const TokenType type);

#endif // FE_TOKEN_TREE_CFG_H_
