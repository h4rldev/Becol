#ifndef LIBBECOL
#define LIBBECOL

#include "memory/mem.h"

void BecolInit(void);
char *BecolVersion(void);

void BecolModuleSearchPaths(char *paths[], int paths_len);
void BecolRun(MemoryArena *arena
              //, const char *path
);

void BecolCleanup(MemoryArena *arena);

#endif
