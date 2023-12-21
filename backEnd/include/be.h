#ifndef BACK_END_H_
#define BACK_END_H_

enum BackEndError
{
    #define DEF_BE_ERR(err, msg) BE_ERR_ ## err,

        #include "be_errs_codegen.inc"

    #undef  DEF_BE_ERR
};

const char* beGetErrorMsg(BackEndError err);

#endif // BACK_END_H_