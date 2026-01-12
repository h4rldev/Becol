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
    // int arg_num = 1;
    // char *file = NULL;

    MemoryArena *arena = BecolArenaCreate(MiB(64), MiB(1));

    // #ifdef MODULE_SEARCH_LAUNCH_ARG
    // char *module_paths[128];
    // int module_paths_end = 0;
    // #endif

    // BecolInit();

    printf("Using libbecol version %s. Made by Carson Coder and Licensed under "
           "the AGPL 3.0 License\n",
           BecolVersion());
    /* while (arg_num < argc) {
         char *arg = argv[arg_num];
         if (arg[0] != '-') {
             if (file == NULL)
                 file = arg;
             else {
                 printf("Multiple files specified\n");
                 return 1;
             }
         } else if ((arg[1] == '-' && strcmp(arg, "--load") == 0) ||
                    (arg[1] != '-' && strcmp(arg, "-l") == 0)) {
             // #ifdef MODULE_SEARCH_LAUNCH_ARG

             if (module_paths_end >= 128) {
                 printf("Maximum 128 search paths\n");
                 return 1;
             }
             module_paths[module_paths_end++] = arg; */

    /*
    #else
    printf("The --load argument as been disabled in this build\n");
    exit(1);
    #endif
    */
    /*
            } else if ((arg[1] == '-' && strcmp(arg, "--help") == 0) ||
                       (arg[1] != '-' && strcmp(arg, "-h") == 0)) {
                printf(
                    "becol [options] file\n"
                    "options:\n"
                    // #ifdef MODULE_SEARCH_LAUNCH_ARG
                    "  --load -l    load module if .so / .dll is specified or\n"
                    "               load modules in folder if a folder is
       passed\n"
                    // #endif
                    "  --help -h    print this message\n");
            } else {
                printf("Extra args\n");
                return 1;
            }
            arg_num++;
        }*/
    /*
    if (file == NULL) {
        printf("No file specified\n");
        return 1;
    }
    FILE *input_file = fopen(file, "r");
    if (input_file == NULL) {
        perror("Error while opening file");
        return 1;
    }

    struct stat buffer;
    if (fstat(fileno(input_file), &buffer) == -1) {
        perror("fstat");
        return 1;
    }
    char *file_data = malloc(buffer.st_size + 1);
    file_data[buffer.st_size] = '\0';
    if (fread(file_data, 1, buffer.st_size, input_file) != buffer.st_size ||
        strlen(file_data) != buffer.st_size) {
        printf("Read more or less data than expected or a null byte is in the "
               "file\n");
        return 1;
    }
    fclose(input_file);
#ifdef MODULE_SEARCH_LAUNCH_ARG
    BecolModuleSearchPaths(module_paths, module_paths_end);
#endif*/
    char *file_data = {0};
    int ret = BecolParseArgs(arena, argc, argv, &file_data);
    if (ret != 0)
        return ret;

    BecolRun(arena, file_data);

    // cleanup
    BecolCleanup(arena);
}
