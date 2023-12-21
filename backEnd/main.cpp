#include "./include/be.h"
#include "../common/binaryTree/tree_saveLoad.h"
#include "../frontEnd/include/fe.h"
#include "../common/binaryTree/tree_graphDump.h"

const char*  LOG_FILE_NAME = "logs.html";
const char* TREE_FILE_NAME = "output.tre";
const char*  OUT_FILE_NAME = "ass.bnn";

int main()
{
    BackEndError error  =     BE_ERR_NO;
    TreeError treeError = TREE_ERROR_NO;
    BackEnd be = {};
    FrontEnd fe = {};

    FILE* logFile  = nullptr;
    FILE* loadFile = nullptr;
    FILE* outFile  = nullptr;

    Tree tree = {};

    logFile = logOpenFile(LOG_FILE_NAME);
    if (!logFile)
    {
        error = BE_ERR_BAD_FOPEN;
        goto ReturnBadLogFile;
    }
    frontEndSetLogFile    (logFile);
    treeSaveLoadSetLogFile(logFile);
    backEndSetLogFile     (logFile);

    frontEndCtor(&fe, nullptr);
    if (fe.error)
    {
        error = BE_ERR_FE;
        goto ReturnBadFrontEndCtor;
    }

    loadFile = fopen(TREE_FILE_NAME, "r");
    if (!loadFile)
    {
        error = BE_ERR_BAD_FOPEN;
        goto ReturnBadLoadFile;
    }

    tree = treeLoadFromFile(loadFile, &fe, &treeError);
    if (!tree.rootBranch)
    {
        error = BE_ERR_TREE;
        goto ReturnBadTree;
    }

    outFile = fopen(OUT_FILE_NAME, "w");
    if (!outFile)
    {
        error = BE_ERR_BAD_FOPEN;
        goto ReturnBadOut;
    }

    backEndCtor(&be);

    treeGraphDump(&tree);

    translateGlobalVars(&be, tree.rootBranch, outFile);
    translationBeginning(outFile);
    translateToAss(&be, tree.rootBranch, outFile);

    fclose(outFile);
    ReturnBadOut:
    treeDtor(&tree);
    ReturnBadTree:
    fclose(loadFile);
    ReturnBadLoadFile:
    frontEndDtor(&fe);
    ReturnBadFrontEndCtor:
    fclose(logFile);
    ReturnBadLogFile:
    return error;
}