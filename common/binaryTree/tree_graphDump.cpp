#include "tree_graphDump.h"

#include <assert.h>

#include "../../libs/logs/logs.h"
#include "../../frontEnd/include/fe.h"

static const char BGR_COLOR[]  = "#FFFFFF";
static const char MAIN_COLOR[] = "#FF00FF";
static const char SCD_COLOR[]  = "#F3D681";

static const char DUMP_DIR[]  = "./dumps";

static void makeLogDir(const char* dir)
{
    const int cmdBufferSize = 150;

    const char cmdMask[] = "mkdir %s -p";

    char cmdBuffer[cmdBufferSize] = {};
    snprintf(cmdBuffer, cmdBufferSize, cmdMask, dir);

    system(cmdBuffer);
}


static void compileDot(const char* fileName)
{
    const int cmdBufferSize = 150;

    const char cmdMask[] = "dot %s -Tpng -o %s.png";

    char cmdBuffer[cmdBufferSize] = {};
    snprintf(cmdBuffer, cmdBufferSize, cmdMask, fileName, fileName);

    system(cmdBuffer);
}

#define log(...) fprintf(dotFile, __VA_ARGS__)

static void printNodes_recursive(TreeNode* node, int* nNode, FILE* dotFile)
{
    if (!node)
        return;

    int curNode = *nNode;
    int len = 0;
    const char* tokenStr = getTokenStr(&node->data, &len);

    log("\"%p%d\" [shape = circle, style = filled, fillcolor = \"%s\", color = \"%s\", ", 
                            node, curNode, MAIN_COLOR, SCD_COLOR);
    log("label = \"");

    log("%.*s", len, tokenStr);

    log("\"];\n");

    if (node->leftBranch != NULL)
    {
        (*nNode)++;
        log("\"%p%d\" -> \"%p%d\"[color = \"%s\", weight = 1]\n",
                                        node, curNode, node->leftBranch, *nNode, SCD_COLOR);
        printNodes_recursive(node->leftBranch, nNode, dotFile);
    }
    if (node->rightBranch != NULL)
    {
        (*nNode)++;
        log("\"%p%d\" -> \"%p%d\"[color = \"%s\", weight = 1]\n",
                                        node, curNode, node->rightBranch, *nNode, SCD_COLOR);
        printNodes_recursive(node->rightBranch, nNode, dotFile);
    }
}

void treeGraphDump(Tree* tree)
{
    assert(tree);
    if (!tree)
        return;

    static int nDump = 0;

    if (nDump == 0)
        makeLogDir(DUMP_DIR);

    const int fileNameSize = 50;
    char fileName[fileNameSize] = {};
 
    getCurrentTimeStr(fileName, fileNameSize);

    const int fileDirSize = 100;
    char fileDir[fileDirSize] = {};

    snprintf(fileDir, fileDirSize, "%s/%d_%s.dot", DUMP_DIR, nDump, fileName);

    FILE* dotFile = fopen(fileDir, "w");
    if (!dotFile)
        return;

    int nNodes = 0;
    log("digraph G{\n"
        "rankdir = TB;\n"
        "bgcolor = \"%s\";\n", BGR_COLOR);
    printNodes_recursive(tree->rootBranch, &nNodes, dotFile);
    log("}");

    fclose(dotFile);

    compileDot(fileDir);
}

#undef log