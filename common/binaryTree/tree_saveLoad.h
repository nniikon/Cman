#ifndef TREE_SAVE_LOAD_H_
#define TREE_SAVE_LOAD_H_

#include "tree.h"
#include "../../frontEnd/include/fe.h"

Tree treeLoadFromFile(FILE* file, FrontEnd* fe, TreeError* err);

void treeSaveLoadSetLogFile(FILE* file);

#endif // TREE_SAVE_LOAD_H_