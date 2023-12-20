#include "../include/fe.h"
#include "../../libs/fileToBuffer/fileToBuffer.h"

// delete FIXME


// delete

const char* LOG_FILE_NAME = "logs.html";
const char* INPUT_FILE_NAME = "source.cmk";

int main()
{
    FILE* logFile = logOpenFile(LOG_FILE_NAME);
    if (!logFile)
        return -1;

    FrontEnd fe = {};

    frontEndSetLogFile(logFile);

    frontEndCtor(&fe, INPUT_FILE_NAME);
    if (fe.error)
        return fe.error;

    char* input = (char*) ftbPutFileToBuffer(nullptr, fopen(INPUT_FILE_NAME, "r"));

    Stack tokens = parseStrToTokens(&fe, input);

    Tree syntaxTree = parseTokensToSyntaxTree(&fe, &tokens);

    printSyntaxErrors(&fe, &tokens);

    stackDtor(&tokens);
    treeDtor(&syntaxTree);
    free(input);

    frontEndDtor(&fe);
    if (fe.error)
        return fe.error;
}