// libbecol. The library to parse and run becol code
#include "memory/mem.h"

#include "defines.h"
#include "libbecol.h"
#include "tokenizer/tokenizer.h"
#include <stdio.h>

void BecolInit() {
    return; // TODO: things
}

char *BecolVersion() { return (VERSION " " SOURCE "+" COMMIT); }

void BecolModuleSearchPaths(char *paths[], int paths_len) {
    for (int i = 0; i < paths_len; i++) {
        printf("Loading modules from %s\n", paths[i]);
    }
}

void BecolRun(MemoryArena *arena, const char *str) {
    ASTTreeNode *root = BecolGenASTTree(arena, (char *)str);
    BecolFreeASTNode(arena, root);
}

void BecolCleanup(MemoryArena *arena) { BecolArenaDestroy(arena); }
