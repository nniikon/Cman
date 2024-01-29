#include "stoYACC.h"

#include <assert.h>
#include <ctype.h>

#include "../libs/fileToBuffer/fileToBuffer.h"
#include "../libs/logs/logs.h"
#include "../frontEnd/include/fe.h"

static FILE* logFile = nullptr;

const char* stoyaccGetErrorMsg(StoyaccError err)
{
        #define DEF_STOYACC_ERR(err, msg)                                      \
            case STOYACC_ERR_ ## err:                                          \
                return msg;

        switch (err)
                {
                    #include "stoYACC_errs_codegen.inc"
                    default:
                        return "No such error was found";
                }
        #undef DEF_STOYACC_ERR 
}


enum StoyaccTokenNode
{
    STOYACC_TOKEN_RETURN,
    STOYACC_TOKEN_LEFT,
    STOYACC_TOKEN_RIGHT,
    STOYACC_TOKEN_TEMP,
};


struct StoyaccTokenInfo
{
    StoyaccTokenNode node;
    bool isNew;
};


#define STOYACC_LOG_RETURN_ERROR(err)                                          \
    do {                                                                       \
        LOGF_ERR(logFile, "%s\n", stoyaccGetErrorMsg(err));                    \
        return err;                                                            \
    } while (0)


#define STOYACC_LOG_COLOR(color, ...)                                          \
    LOGF_COLOR(logFile, color, __VA_ARGS__);


#define print(...) fprintf(file, __VA_ARGS__)

// The length of each debug message.
const int STOYACC_MAX_LENGTH = 50; // i couldn't resist

// FIXME: copypaste that i don't really want to fix =(

const char SUBFUNCTION_DECLARATION_MASK[] =
    "static TreeNode* %s_%d"
    "(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)\n";


const char FUNCTION_DECLARATION_MASK[] =
    "static TreeNode* %s"
    "(FrontEnd* fe, Tree* tree, Stack* tokensStk, unsigned int* pos)\n";


const char CREATE_NODE_MASK[] = 
    "\t%s = treeCreateNode(tree, nullptr, nullptr, nullptr, tokenAt(*pos));\n";


const char SUBFUNCTION_BEGINNING[] = 
    "{                                   \n"
    "\tASSERT_ALL();                     \n"
    "\tbool isErrorReal = false;       \n\n"
    "\tconst char* errStr = \"\";      \n\n"
    "\tTreeNode*  returnNode = nullptr;  \n"
    "\tTreeNode* currentNode = nullptr;  \n"
    "\tTreeNode*    leftNode = nullptr;  \n"
    "\tTreeNode*   rightNode = nullptr;  \n"
    "\tTreeNode*    tempNode = nullptr;\n\n";


const char FUNCTION_BEGINNING[] = 
    "{                                  \n"
    "\tASSERT_ALL();                    \n"
    "\tunsigned int oldPos = *pos;      \n"
	"\tTreeNode* returnNode = nullptr;\n\n";


#define ERROR_HANDLING                                                         \
    "\t{                                  \n"                                  \
    "\t\tif (isErrorReal)                 \n"                                  \
    "\t\t{                                \n"                                  \
    "\t\t\tpushSyntaxError(*pos, errStr); \n"                                  \
    "\t\t}                                \n"                                  \
    "\t\treturn nullptr;                  \n"                                  \
    "\t}                                  \n"


// Copypaste but at least i can understand what's going on in here
// Should've written it in python for sure.
// Why would anyone write this in c.
#define SET_SYNTAX_ERR_SYMBOL_MASK                                             \
    "\terrStr = \"expected a \\\'%c\\\'\";\n"

#define SET_SYNTAX_ERR_KEYWORD_MASK                                            \
    "\terrStr = \"expected a \\\'%s\\\'\";\n"

#define SET_SYNTAX_ERR_FUNC_MASK                                               \
    "\terrStr = %s_errStr;\n"

const char FUNCTION_ERROR_STR_MASK[] = 
    "const char* %s_errStr = \"expected %s\";\n";

const char KEYWORD_NODE_MASK[] =
    "\t// <%s>                         \n"
        SET_SYNTAX_ERR_KEYWORD_MASK
    "\tif (!isKeyword(KEY_WORD_%s))    \n"
        ERROR_HANDLING
    "\tcreateNode(returnNode);         \n"
    "\tcurrentNode = returnNode;       \n"
    "\t(*pos)++;                     \n\n";


const char SYMBOL_NODE_MASK[] =
    "\t// '%c'                        \n"
        SET_SYNTAX_ERR_SYMBOL_MASK
    "\tif (!isOperation(OPER_TYPE_%s))\n"
        ERROR_HANDLING
    "\t(*pos)++;                    \n\n";


const char PUNC_NODE_MASK[] =
    "\t// '%c'                          \n"
        SET_SYNTAX_ERR_SYMBOL_MASK 
    "\tif (!isPunctuation(PUNC_TYPE_%s))\n"
        ERROR_HANDLING
    "\t(*pos)++;                      \n\n";


const char WHITE_SPACE_MASK[] = 
     "\t// _                                         \n"
     "\terrStr = \"missing space\";                  \n"
     "\tif (!getWhiteSpace(fe, tree, tokensStk, pos))\n"
        ERROR_HANDLING
     "                                               \n";


const char FUNCTION_CALL_MASK[] = 
    "\t// %s()                           \n"
        SET_SYNTAX_ERR_FUNC_MASK 
    "\t%s = %s(fe, tree, tokensStk, pos);\n"
    "\tif (!%s)                          \n"
        ERROR_HANDLING
    "                                    \n";


const char ERROR_HANDLING_ON[] = 
    "\tisErrorReal = !isErrorReal;     \n\n";


//FIXME: K O C T bI JL b
// #define cringe
const char CYCLE_CALL_BEGINNING[] = 
    "\t// Cycle started                     \n"
    "\tcurrentNode = createSemicolonNode(); \n"
    "\treturnNode = currentNode;            \n"
    "\twhile (*pos < tokensStk->size)       \n"
    "\t{                                    \n"
    "\tunsigned int oldPos = *pos;          \n"
    "#define return break                   \n"
    "#define nullptr                      \n\n";


//FIXME: K O C T bI JL b
// #define cringe
const char CYCLE_CALL_ENDING[] = 
    "\tcurrentNode-> leftBranch = tempNode;             \n"
    "\tcurrentNode->rightBranch = createSemicolonNode();\n"
    "\tcurrentNode = currentNode->rightBranch;          \n"
    "#undef return                                      \n"
    "#undef nullptr                                     \n"
    "\t}                                              \n\n";


const char TRY_SUBFUNCTION_MASK[] = 
    "\tif (!returnNode)                                 \n"
    "\t{                                                \n"
    "\t\t*pos = oldPos;                                 \n"
    "\t\treturnNode = %s_%d(fe, tree, tokensStk, pos);  \n"
    "\t}                                              \n\n";


const char SUBFUNCTION_ENDING[] =
    "\tif (!returnNode->rightBranch)\n"
    "\t\treturnNode->rightBranch = rightNode;\n"
    "\tif (!returnNode->leftBranch)\n"
    "\t\treturnNode-> leftBranch =  leftNode;\n"
    "\treturn returnNode;\n"
    "}                 \n\n";


const char FUNCTION_ENDING[] = 
    "\treturn returnNode;\n"
    "}                 \n\n";


void stoyaccSetLogFile(FILE* file)
{
    logFile = file;
}


static char* skipSpaces(char* src)
{
    while (isspace(src[0]))
        src++;

    return src;
}


static int getWordLen(char* src)
{
    char* start = src;
    while(!isspace(src[0]) && src[0])
        src++;

    return (int)(src - start);
}


static const char* stoyaccGetNodeName(const StoyaccTokenNode node)
{
    switch (node)
    {
        // FIXME: a lot of copypaste.

        case STOYACC_TOKEN_LEFT:   return   "leftNode";
        case STOYACC_TOKEN_RIGHT:  return  "rightNode";
        case STOYACC_TOKEN_RETURN: return "returnNode";
        case STOYACC_TOKEN_TEMP:   return   "tempNode";

        default:
            assert(0);
            return nullptr;
    }
}


// Handles anything in ' '
static bool stoyaccTryPrintSymbol(FILE* file, char** str,
                                                   const StoyaccTokenInfo* info)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Trying to find a new symbol with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    if ((*str)[0] != '\'' || (*str)[2] != '\'')
        return false;

    char symbol = (*str)[1];
    const char* operName = nullptr;
    const char* puncName = nullptr;

   switch (symbol)
    {
        case ')': operName =       "RBR"; break;
        case '(': operName =       "LBR"; break;
        case '=': operName =      "ASGN"; break;
        case '*': operName =       "MUL"; break;
        case '/': operName =       "DIV"; break;
        case '%': operName =       "RMD"; break;
        case '+': operName =       "ADD"; break;
        case '-': operName =       "SUB"; break;
        case ';': puncName = "SEMICOLON"; break;
        case '{': puncName =  "FIGURE_L"; break;
        case '}': puncName =  "FIGURE_R"; break;

        default:
            STOYACC_LOG_COLOR(red, "Unable to recognize the symbol\n");
            return false;
    }

    STOYACC_LOG_COLOR(green, "A new symbol was found: %c\n", symbol);
    
    if (info->isNew)
        print(CREATE_NODE_MASK, stoyaccGetNodeName(info->node));

    if (operName)
        print(SYMBOL_NODE_MASK, symbol, symbol, operName);

    if (puncName)
        print(PUNC_NODE_MASK,   symbol, symbol, puncName);

    *str = *str + 3;

    return true;
}


static void stoyaccPrintKeyword(FILE* file, char* str)
{
    print(KEYWORD_NODE_MASK, str, str, str);
}


// Handles anything in < >
static bool stoyaccTryPrintKeyword(FILE* file, char** str,
                                                   const StoyaccTokenInfo* info)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Trying to find a new keyword with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    if ((*str)[0] != '<')
        return false;

    (*str)++;

    char* strEnd = strchr(*str, '>');
    if (!strEnd)
        return false;
    // It's better to not \0 the string and use its length but im way too lazy.
    *strEnd = '\0';

     STOYACC_LOG_COLOR(green, "A new keyword was found: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    if (info->isNew)
        print(CREATE_NODE_MASK, stoyaccGetNodeName(info->node));

    stoyaccPrintKeyword(file, *str);

    *str = strEnd + 1;

    return true;
}


// Handles white spaces (_)
static bool stoyaccTryPrintWS(FILE* file, char** str)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Trying to find a WS with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    if ((*str)[0] != '_')
        return false;

    print(WHITE_SPACE_MASK);

    (*str)++;

    return true;
}


static void stoyaccPrintFunctionCall(FILE* file, char** str,
                                                    const StoyaccTokenNode node)
{
    const char* name = stoyaccGetNodeName(node);

    print(FUNCTION_CALL_MASK, *str, *str, name, *str, name);
}


// Handles function calls
static bool stoyaccTryPrintFunc(FILE* file, char** str, 
                                                   const StoyaccTokenInfo* info)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Trying to find a new function call with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    // FIXME: Ideally it should check if the function has been defined before
    //        But that would require too much effort so I'll prob never do it.
    //
    //        UPD 2 days later: definitely not doing this.
    //        UPD 5 days later: never doing this

    if (!isalpha((*str)[0]))
        return false;

    int nameLen = getWordLen(*str);
    (*str)[nameLen] = '\0';

    STOYACC_LOG_COLOR(green, "A new function call was found: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    if (info->isNew)
        print(CREATE_NODE_MASK, stoyaccGetNodeName(info->node));

    stoyaccPrintFunctionCall(file, str, info->node);
    
    *str = *str + nameLen + 1;

    return true;
}


static StoyaccTokenInfo stoyaccSetUpTokenInfo(char** str)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Setting up a token: %.*s\n", 10, *str);

    // Standart settings.
    StoyaccTokenInfo info = {.node = STOYACC_TOKEN_RETURN, .isNew = false};

    if ((*str)[0] != '[')
    {
        return info;
    }
    (*str)++;

    while ((*str)[0] != ']')
    {
        switch ((*str)[0])
        {
            case 'C':
                info.isNew = true;
                break;

            case 'l':
                info.node = STOYACC_TOKEN_LEFT;
                break;

            case 'r':
                info.node = STOYACC_TOKEN_RIGHT;
                break;

            case 'R':
                info.node = STOYACC_TOKEN_RETURN;
                break;

            default:
                STOYACC_LOG_COLOR(red, "Unable to recognize token setting\n");
                return info;
                break;
        }
        (*str)++;
    }
    (*str)++;

    STOYACC_LOG_COLOR(green, "Settings done   \n"
                             "Direction:    %s\n"
                             "Is token new: %d\n", stoyaccGetNodeName(info.node), info.isNew);

    return info;
}


static bool stoyaccTryPrintErrHandling(FILE* file, char** str)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Trying to turn on error handling.\n");

    if ((*str)[0] != '$')
        return false;
    (*str)++;

    print(ERROR_HANDLING_ON);

    return true;
}


 // FIXME: fix the declarations please
static bool stoyaccTryPrintCycle(FILE* file, char** str);

static bool stoyaccPrintToken(FILE* file, char** str, StoyaccTokenInfo* info)
{ 
    assert(str);
    STOYACC_LOG_COLOR(white, "Found new token with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    *str = skipSpaces(*str);

    StoyaccTokenInfo tempInfo = {};
    if (info == nullptr)
    {
        tempInfo = stoyaccSetUpTokenInfo(str);
        info = &tempInfo;
    }
    
    *str = skipSpaces(*str);

    if (!stoyaccTryPrintSymbol     (file, str, info) && 
        !stoyaccTryPrintKeyword    (file, str, info) &&
        !stoyaccTryPrintWS         (file, str      ) &&
        !stoyaccTryPrintCycle      (file, str      ) &&
        !stoyaccTryPrintFunc       (file, str, info) &&
        !stoyaccTryPrintErrHandling(file, str      )   )
    {
        STOYACC_LOG_COLOR(red, "Couldn't recognize a new token with: %.*s\n",
                                             (int)STOYACC_MAX_LENGTH / 3, *str);
        return false;
    }

    return true;
}


static bool stoyaccPrintToken(FILE* file, char** str)
{
    return stoyaccPrintToken(file, str, nullptr);
}


static bool stoyaccTryPrintCycle(FILE* file, char** str)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Trying to find new cycle with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    if ((*str)[0] != '(')
        return false;

    (*str)++;    

    char* endStr = strchr(*str, ')');
    if (endStr == nullptr)
        return false;

    StoyaccTokenInfo info = {.node = STOYACC_TOKEN_TEMP, .isNew = false};

    switch(endStr[1])
    {
        case '*':
            print(CYCLE_CALL_BEGINNING);

            while(stoyaccPrintToken(file, str, &info))
                ;

            print(CYCLE_CALL_ENDING);
            break;

        default:
            return false;
    }

    *str = endStr + 2;

    return true;
}


static StoyaccError stoyaccGenSubfunction(FILE* file, char* funcName,
                                          char** str, int*  order)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "Found new subfunction with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

    print(SUBFUNCTION_DECLARATION_MASK, funcName, (*order)++);
    print(SUBFUNCTION_BEGINNING);

    while (stoyaccPrintToken(file, str))
        ;

    if ((*str)[0] != ';' && (*str)[0] != ':' && (*str)[0] != '|')
        return STOYACC_ERR_SYNTAX;

    print(SUBFUNCTION_ENDING);

    return STOYACC_ERR_NO; 
}


static StoyaccError stoyaccGenFunction(FILE* file, FILE* headerFile, char** str)
{
    assert(str);
    STOYACC_LOG_COLOR(white, "getFunction started with: <%.*s>\n",
                                                      STOYACC_MAX_LENGTH, *str);

    int order   = 0;
    *str        = skipSpaces(*str);
    int nameLen = getWordLen(*str);

    // Null-terminate the string
    (*str)[nameLen] = '\0';

    char* functionName = *str;
    *str = skipSpaces(*str + nameLen + 1);

    // By default error string will be just function name.
    char* errStr = functionName;
    char* errStrEnd = nullptr;
    if ((*str)[0] == '\"')
    {
        (*str)++;
        errStr = *str;

        errStrEnd = strchr(*str, '\"');
        if (!errStrEnd)
            STOYACC_LOG_RETURN_ERROR(STOYACC_ERR_SYNTAX);

        errStrEnd[0] = '\0';

        (*str) = skipSpaces(errStrEnd + 1);
    }
    
    while ((*str)[0] == ':' || (*str)[0] == '|')
    {
        STOYACC_LOG_COLOR(white, "Found new subfunction with: %.*s\n",
                                                      STOYACC_MAX_LENGTH, *str);

        *str = skipSpaces(*str + 1);
        StoyaccError err = stoyaccGenSubfunction(file, functionName, str, &order);
        if (err)
            STOYACC_LOG_RETURN_ERROR(err);
    }

    if ((*str)[0] != ';')
        return STOYACC_ERR_SYNTAX;
    (*str)++;

    fprintf(headerFile, FUNCTION_ERROR_STR_MASK,   functionName, errStr);
    fprintf(headerFile, FUNCTION_DECLARATION_MASK, functionName);
    fprintf(headerFile, ";");

    print(FUNCTION_DECLARATION_MASK, functionName);
    print(FUNCTION_BEGINNING);

    for (int i = 0; i < order; i++)
        print(TRY_SUBFUNCTION_MASK, functionName, i);

    print(FUNCTION_ENDING);

    return STOYACC_ERR_NO; 
}


StoyaccError stoyaccGenCompiler(FILE* inFile, FILE* outFile, FILE* headerFile)
{
    assert(inFile);

    if (!inFile)
        STOYACC_LOG_RETURN_ERROR(STOYACC_ERR_NULLPTR_PASSED);

    char* str = (char*) ftbPutFileToBuffer(nullptr, inFile);
    char* strCpy = str;

    if (!str)
        STOYACC_LOG_RETURN_ERROR(STOYACC_ERR_BAD_FTB);

    while (str[0])
    {
        stoyaccGenFunction(outFile, headerFile, &str);
    }
    
    free(strCpy);

    return STOYACC_ERR_NO;
}



