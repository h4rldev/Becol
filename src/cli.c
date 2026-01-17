#include "cli.h"
#include "libbecol/libbecol.h"
#include "libbecol/memory/mem.h"
#include "sys/stat.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>

void print_usage(void) {
    printf("Usage: becol [<options>] [file]\n"
           "Options:\n"
           "   --load -l    load module if .so / .dll is specified or load "
           "modules in provided path\n"
           "   --help -h    print this message\n");
}

int BecolParseArgs(MemoryArena *arena, int argc, char *argv[],
                   char **file_data) {
    int option_index = 0;
    int module_paths_idx = 0;
    char arg = 0;
    char *path = {0};
    char *module_paths[1024] = {0};
    FILE *input_file = NULL;

    const struct option long_options[] = {
        {"load", required_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
    };

    if (argc < 2) {
        print_usage();
        return 1;
    }

    while ((arg = getopt_long(argc, argv, ":l:h", long_options,
                              &option_index)) != -1) {
        switch (arg) {
        case 'l':
            module_paths[module_paths_idx] =
                BecolArenaPushArray(arena, char, strlen(optarg) + 1, false);
            memcpy(module_paths[module_paths_idx], optarg, strlen(optarg) + 1);
            break;
        case 'h':
            print_usage();
            return 1;

        case '?':
            fprintf(stderr, "Unknown option: %c\n", optopt);
            print_usage();
            return 1;

        default:
            fprintf(stderr, "Unknown error, getopt returned character: 0%o\n",
                    arg);
            return 1;
        }
    }

    if (optind != argc - 1) {
        fprintf(stderr, "Missing file path argument\n");
        print_usage();
        return 1;
    }

    path = argv[optind];
    // printf("Opening file at path: %s\n", path);
    input_file = fopen(path, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error while opening file at path: %s\n", path);
        return 1;
    }

    struct stat buffer;
    if (fstat(fileno(input_file), &buffer) < 0) {
        perror("fstat");
        return 1;
    }

    // printf("File size: %ld\n", buffer.st_size);

    *file_data = BecolArenaPushArray(arena, char, buffer.st_size + 1, false);

    if (fread(*file_data, 1, buffer.st_size, input_file) !=
        (unsigned long)buffer.st_size) {
        perror("fread");
        return 1;
    }

    *file_data[buffer.st_size] = '\0';

    // printf("File data: %s", *file_data);

    if (module_paths_idx != 0)
        BecolModuleSearchPaths(module_paths, module_paths_idx);

    return 0;
}
