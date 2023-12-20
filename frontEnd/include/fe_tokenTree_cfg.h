#ifndef FE_TOKEN_TREE_CFG_H_
#define FE_TOKEN_TREE_CFG_H_

enum TokenType
{
    TOKEN_OPERATION,   //<  +, -, *.
    TOKEN_IDENTIFIER,  //< function names, variable names.
    TOKEN_KEY_WORD,    //< if, else, for, while.
    TOKEN_COMMENT,     //< // this is a comment.
    TOKEN_WHITE_SPACE, //< ' ', '\t', '\n'.
    TOKEN_PUNCTUATION, //< ',', ';', '{', '}'.
    TOKEN_NUMBER,      //< 5, 1e6, -12, ...
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

#endif // FE_TOKEN_TREE_CFG_H_