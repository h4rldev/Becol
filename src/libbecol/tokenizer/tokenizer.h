
#ifndef BECOL_TOKENIZER
#define BECOL_TOKENIZER

#include "../memory/mem.h"
#include "asttree.h"
#include "tokens.h"

ASTTreeNode *BecolGenASTTree(MemoryArena *arena, char *prog);

#endif
