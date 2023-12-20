#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "tokenCommon.h"

enum OperationAssoc // associativity
{
    OPER_LEFT_TO_RIGHT,
    OPER_RIGHT_TO_LEFT,
};

enum OperationArity
{
    OPER_NONEARY  = 1 << 0,
    OPER_UNARY    = 1 << 1,
    OPER_BINARY   = 1 << 2,
    OPER_TERNARY  = 1 << 3,
};

enum OperationType
{
    #define DEF_OPERATIONS(type, ...) OPER_TYPE_ ## type,
        #include "operations_codegen.inc"
    #undef  DEF_OPERATIONS
};

struct Operation
{
    TokenInfo info;
    OperationType  type;
    const int   order;
    const int   arity;
    OperationAssoc assoc;
};

const Operation OPERATIONS[] = 
{
    #define DEF_OPERATIONS(type, name, order, assoc, arity)\
        {{name, sizeof(name) - 1}, OPER_TYPE_ ## type, order, arity, OPER_ ## assoc},

        #include "operations_codegen.inc"

    #undef  DEF_OPERATIONS
};

const int N_OPERATIONS = sizeof(OPERATIONS) / sizeof(OPERATIONS[0]);

const int OPERATION_EXPRESSION_ORDER = 12;

#endif // OPERATORS_H_