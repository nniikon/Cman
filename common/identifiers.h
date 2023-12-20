#ifndef IDENTIFIERS_H_
#define IDENTIFIERS_H_

#include "tokenCommon.h"

enum IdentifierType
{
    IDENTIFIER_VAR,
    IDENTIFIER_FUNC,
};

struct Identifier
{
    TokenInfo info;
    //IdentifierType type;
    unsigned int nameTableID;
};

#endif // IDENTIFIERS_H_