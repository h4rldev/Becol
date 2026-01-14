#ifndef BECOL_CLI
#define BECOL_CLI

#include "libbecol/memory/mem.h"

int BecolParseArgs(MemoryArena *arena, int argc, char *argv[],
                   char **file_data);

#endif
