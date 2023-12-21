#include "./include/fe.h"
#include "../libs/fileToBuffer/fileToBuffer.h"
#include "../common/binaryTree/tree_saveLoad.h"

const char* LOG_FILE_NAME    = "logs.html";
const char* INPUT_FILE_NAME  = "source.cmk";
const char* OUTPUT_FILE_NAME = "output.tre";

int main()
{
    FrontEndError error = FE_ERR_NO;

    FILE* logFile    = nullptr;
    FILE* srcFile    = nullptr;
    FILE* outputFile = nullptr;

    char* srcInput = nullptr;

    FrontEnd fe     = {};
    Stack tokens    = {};
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

    outputFile = fopen(OUTPUT_FILE_NAME, "w");
    if (!outputFile)
    {
        error = FE_ERR_BAD_FOPEN;
        goto ReturnBadOutputFile;
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
        goto ReturnBadTokens;
    }

    syntaxTree = parseTokensToSyntaxTree(&fe, &tokens);
    if (fe.error)
    {
        error = fe.error;
        goto ReturnBadTree;
    }

    printSyntaxErrors(&fe, &tokens);
    treeSaveToFile(&syntaxTree, outputFile);

    treeDtor(&syntaxTree);
    ReturnBadTree:
    stackDtor(&tokens);
    ReturnBadTokens:
    free(srcInput);
    ReturnBadFtb:
    frontEndDtor(&fe);
    ReturnBadCtor:
    fclose(outputFile);
    ReturnBadOutputFile:
    fclose(srcFile);
    ReturnBadSrcFile:
    fclose(logFile);
    ReturnBadLogFile:
    return error;
}