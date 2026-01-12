#include "../config.h"
#include "../memory/mem.h"
#include "tokens.h"

#ifndef BECOL_ASTTREE
    #define BECOL_ASTTREE

    #ifdef INCLUDE_ASTTREE_TOKEN_NAMES
extern char *ASTTreeTypeNames[];
    #endif

typedef enum {
    BECOL_AST_PROGRAM,
    BECOL_AST_ACTION,  //(forced: bool, module: string, action: string)
    BECOL_AST_REQUEST, //(module: string, for: string)
    BECOL_AST_TELL,    //(module: string)

    BECOL_AST_LITERAL,  //(val: any)
    BECOL_AST_VARIABLE, //(name: string)
    BECOL_AST_GROUPING, // eg. 5 + 1

    BECOL_AST_ADD,
    BECOL_AST_SUB,
    BECOL_AST_MUL,
    BECOL_AST_DIV,
    BECOL_AST_MOD,
    BECOL_AST_EQUAL,
    BECOL_AST_GREATOR,
    BECOL_AST_LESS,
    BECOL_AST_GREATER_OR_EQUAL,
    BECOL_AST_LESS_OR_EQUAL,
    BECOL_AST_NOT_EQUAL,
} ASTTreeType;

typedef struct ASTTreeNode {
    ASTTreeType type;
    struct ASTTreeNode **child_nodes;
    void **arguments; // depends on type, most of the time null but not null for
                      // things with values
    int num_of_child_nodes;
    int num_of_arguments;
} ASTTreeNode;

char *BecolGetTypeNameFromASTType(MemoryArena *arena, ASTTreeType type);

ASTTreeNode *BecolAllocASTNode(MemoryArena *arena);
void BecolFreeASTNode(MemoryArena *arena, ASTTreeNode *node);

#endif
