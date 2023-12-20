#ifndef FRONT_END_SYNTAX_ERRORS_H_
#define FRONT_END_SYNTAX_ERRORS_H_

enum SyntaxErrorType
{
    #define DEF_SYN_ERR(type, err) SYNTAX_ERR_ ## type,

        #include "fe_syntaxErrs_codegen.inc"

    #undef DEF_SYN_ERR
};

struct SyntaxError
{
    unsigned int tokenPos;
    SyntaxErrorType type;
};

const char* getSyntaxError(SyntaxErrorType err);

#endif // FRONT_END_SYNTAX_ERRORS_H_