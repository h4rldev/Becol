#include "asttree.h"
#include "../config.h"
#include "../memory/mem.h"
#include <stddef.h>
#include <stdio.h>

// utility file for stuff relating to AST Tree stuff.

#ifdef INCLUDE_ASTTREE_TOKEN_NAMES
char *ASTTreeTypeNames[] = {
    "Program",     "Action",   "Request",
    "Tell",

    "Literal",     "Variable", "Grouping",

    "Add",         "Sub",      "Mul",
    "Div",         "Mod",      "Equal",
    "Greator",     "Less",     "GreaterOrEqual",
    "LessOrEqual", "NotEqual",
};
#endif

// if we have the token names, use them. Else just use the number of the token
char *BecolGetTypeNameFromASTType(MemoryArena *arena, ASTTreeType type) {
#ifdef INCLUDE_ASTTREE_TOKEN_NAMES
    return ASTTreeTypeNames[type];
#else
    char *ret = BecolArenaPushArray(arena, char, 5, false);
    snprintf(ret, 5, "%d", type);
    return ret;
#endif
}

ASTTreeNode *BecolAllocASTNode(MemoryArena *arena) {
    ASTTreeNode *node = BecolArenaPushStruct(arena, ASTTreeNode, false);
    node->arguments = NULL;
    node->child_nodes = NULL;
    node->num_of_arguments = 0;
    node->num_of_child_nodes = 0;
    return node;
}

void BecolFreeASTNode(MemoryArena *arena
                      //, ASTTreeNode *node
) {
    BecolArenaPop(arena, sizeof(ASTTreeNode));
}

void _BecolPrintASTTree(MemoryArena *arena, ASTTreeNode *root, int indent) {
    printf("%s\n", BecolGetTypeNameFromASTType(arena, root->type));
    for (int i = 0; i < root->num_of_child_nodes; i++) {
        _BecolPrintASTTree(arena, root->child_nodes[i], indent + 4);
    }
}
void BecolPrintASTTree(MemoryArena *arena, ASTTreeNode *root) {
    _BecolPrintASTTree(arena, root, 0);
}
