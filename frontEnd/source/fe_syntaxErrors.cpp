#include "../include/fe_syntaxErrors.h"


const char* getSyntaxError(SyntaxErrorType err)
{
    #define DEF_SYN_ERR(type, msg) \
        case SYNTAX_ERR_ ## type: return msg;

    switch (err)
    {
        #include "../include/fe_syntaxErrs_codegen.inc"
        default:
            return "No such error was found";
    }

    #undef DEF_SYN_ERR
}