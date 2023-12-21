#include "../include/be.h"

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
