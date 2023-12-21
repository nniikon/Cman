#include "./include/be.h"
#include "../common/binaryTree/tree_saveLoad.h"
#include "../frontEnd/include/fe.h"
#include "../common/binaryTree/tree_graphDump.h"

int main()
{
    BackEndError error  =     BE_ERR_NO;
    TreeError treeError = TREE_ERROR_NO;
    FrontEnd fe = {};


    FILE* logFile = logOpenFile("logs.html");
    if (!logFile)
    {
        error = BE_ERR_BAD_FOPEN;
    }
    frontEndSetLogFile    (logFile);
    treeSaveLoadSetLogFile(logFile);

    frontEndCtor(&fe, "test.HUI");
    if (fe.error)
    {
        error = BE_ERR_FE;
        return error;
    }

    FILE* loadFile = fopen("output.tre", "r");
    if (!loadFile)
        return BE_ERR_BAD_FOPEN;

    Tree tree = treeLoadFromFile(loadFile, &fe, &treeError);

    treeGraphDump(&tree);

    treeDtor(&tree);
    fclose(loadFile);
    frontEndDtor(&fe);
    fclose(logFile);
    return error;
}