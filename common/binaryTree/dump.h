#ifndef DUMP_H_
#define DUMP_H_

#include <stdio.h>
#include "../../libs/logs/logs.h"

// Tree originally was using another logging lib
// So I'm rerouting it to my new lib

#ifndef DUMP_RELEASE
    #define DUMP(file ,...)         LOGF(file, __VA_ARGS__)
    #define DUMP_FUNC_START(file)   LOG_FUNC_START(file);
    #define DUMP_FUNC_SUCCESS(file) LOG_FUNC_END(file);
#else
    #define DUMP              do {} while (0)
    #define DUMP_FUNC_START   do {} while (0)
    #define DUMP_FUNC_SUCCESS do {} while (0)
#endif

#endif // DUMP_H_