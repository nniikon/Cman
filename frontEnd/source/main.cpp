#include "../include/fe.h"
#include "../../libs/fileToBuffer/fileToBuffer.h"

// delete FIXME


// delete

const char* LOG_FILE_NAME = "logs.html";
const char* INPUT_FILE_NAME = "source.cmk";

int main()
{
    FrontEndError error = FE_ERR_NO;
    FILE* logFile = nullptr;
    FILE* srcFile = nullptr;
    FrontEnd fe = {};
    char* srcInput = nullptr;
    Stack tokens = {};
    Tree syntaxTree = {};

    logFile = logOpenFile(LOG_FILE_NAME);
    if (!logFile)
    {
        error = FE_ERR_BAD_FOPEN;
        goto ReturnBadLogFile;
    }

    srcFile = fopen(INPUT_FILE_NAME, "r");
    if (!srcFile)
    {
        error = FE_ERR_BAD_FOPEN;
        goto ReturnBadSrcFile;
    }

    frontEndSetLogFile(logFile);

    frontEndCtor(&fe, INPUT_FILE_NAME);
    if (fe.error)
    {
        error = fe.error;
        goto ReturnBadCtor;
    }

    srcInput = (char*) ftbPutFileToBuffer(nullptr, srcFile);
    if (!srcInput)
    {
        error = FE_ERR_BAD_FTB;
        goto ReturnBadFtb;
    }

    tokens = parseStrToTokens(&fe, srcInput);
    if (fe.error)
    {
        error = fe.error;
        goto ReturnFreeAll;
    }

    syntaxTree = parseTokensToSyntaxTree(&fe, &tokens);
    if (fe.error)
    {
        error = fe.error;
        goto ReturnFreeAll;
    }

    printSyntaxErrors(&fe, &tokens);

    ReturnFreeAll:
    free(srcInput);
    ReturnBadFtb:
    frontEndDtor(&fe);
    ReturnBadCtor:
    fclose(srcFile);
    ReturnBadSrcFile:
    fclose(logFile);
    ReturnBadLogFile:
    return error;
}