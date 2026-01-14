#include "cli.h"
#include "config.h"
#include "libbecol/libbecol.h"
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <sys/stat.h>

// cli interface
// becol [options] file
// options:
//   --load -l    load module if .so / .dll is specified or load modules in
//   folder if a folder is passed

#ifdef _MSC_VER
#define fstat(fd, buf) _fstat(fd, buf)
#define stat _stat;
#endif

int main(int argc, char *argv[]) {
    MemoryArena *arena = BecolArenaCreate(MiB(64), MiB(1));

    // BecolInit();

    printf("Using libbecol version %s. Made by Carson Coder and Licensed under "
           "the AGPL 3.0 License\n",
           BecolVersion());

    char *file_data = {0};
    int ret = BecolParseArgs(arena, argc, argv, &file_data);
    if (ret != 0)
        return ret;

    BecolRun(arena
             // , file_data
    );

    // cleanup
    BecolCleanup(arena);
}
