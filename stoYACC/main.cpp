#include "stoYACC.h"

#include <stdio.h>
#include "../libs/logs/logs.h"

const char    genFileName[] = "stoYACC_generated2.h";
const char headerFileName[] = "stoYACC_generated1.h";
const char  inputFileName[] = "stoYACC_input.styc";

int main()
{
    int retValue = 0;

    StoyaccError err = STOYACC_ERR_NO;
    FILE* logFile = nullptr;
    FILE* inFile = nullptr;
    FILE* genFile = nullptr;
    FILE* headerFile = nullptr;

    logFile = logOpenFile("logs.html");
    if (!logFile)
    {
        retValue = -1;
        goto retBadLogFile;
    }

    inFile = fopen(inputFileName, "r");
    if (!inFile)
    {
        retValue = -1;
        goto retBadInFile;
    }

    genFile = fopen(genFileName, "w");
    if (!genFile)
    {
        retValue = -1;
        goto retBadGenFile;
    }

    headerFile = fopen(headerFileName, "w");
    if (!genFile)
    {
        retValue = -1;
        goto retBadGenHeader;
    }

    stoyaccSetLogFile(logFile);

    err = stoyaccGenCompiler(inFile, genFile, headerFile);
    if (err)
    {
        retValue = (int) err;
        goto retBadGenCompiler;
    }

    retBadGenCompiler:
    fclose(headerFile);

    retBadGenHeader:
    fclose(genFile);

    retBadGenFile:
    fclose(inFile);

    retBadInFile:
    fclose(logFile);

    retBadLogFile:
    return retValue;
}
