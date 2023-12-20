#ifndef SPU_PARSE_ARGS_H
#define SPU_PARSE_ARGS_H

#include <stdlib.h>

struct StrArgument
{
    const char* optName; 
    const char* briefDescr;
    const char* fullDescr;
    const char** output;
    bool isNecessary;
};

struct ConsoleArgs
{
    size_t nStrArgs;
    StrArgument* argsArr;
};

bool parseArgs(int argc, char** argv, ConsoleArgs* args);

#endif