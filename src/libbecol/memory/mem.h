#ifndef BECOL_MEM
#define BECOL_MEM

// headers are fine in headers as long as the headers are include guarded
#include <stdbool.h>
#include <stdint.h>

// Helper macros for number scaling and alignment.
#define KiB(num) ((uint64_t)(num) << 10)
#define MiB(num) ((uint64_t)(num) << 20)
#define GiB(num) ((uint64_t)(num) << 30)
#define ALIGN_UP_TO_POW2(num, pow)                                             \
    (((uint64_t)(num) + ((uint64_t)(pow) - 1)) & (~((uint64_t)(pow) - 1)))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Why is this a linked list when you can just make an arena
/*typedef struct MemoryChunk {
    int size;
    void *addr;
    struct <- gay
MemoryChunk *next;
struct MemoryChunk *prev;
}
MemoryChunk;
*/

typedef struct {
    uint64_t reserved;
    uint64_t committed;
    uint64_t position;
    uint64_t commit_position;
} MemoryArena;

typedef struct {
    uint64_t start_pos;
    MemoryArena *arena;
} TemporaryArena;

#define ARENA_BASE_POS (sizeof(MemoryArena))
#define ARENA_ALIGN (sizeof(void *))

// I was thinking memory should be explicit, instead of implicit.
// extern MemoryChunk *rootMemory;

MemoryArena *BecolArenaCreate(uint64_t reserve_size, uint64_t commit_size);
void BecolArenaDestroy(MemoryArena *arena);
void *BecolArenaPush(MemoryArena *arena, uint64_t size, bool non_zero);
void BecolArenaPop(MemoryArena *arena, uint64_t size);
void BecolArenaPopTo(MemoryArena *arena, uint64_t pos);
void BecolArenaClear(MemoryArena *arena);

TemporaryArena BecolTemporaryArenaBegin(MemoryArena *arena);
void BecolTemporaryArenaEnd(TemporaryArena temp);

TemporaryArena BecolArenaScratchGet(MemoryArena **conflicts,
                                    uint32_t num_conflicts);
void BecolArenaScratchRelease(TemporaryArena scratch);

#define BecolArenaPushStruct(arena, T, nz)                                     \
    (T *)BecolArenaPush(arena, sizeof(T), (bool)nz)
#define BecolArenaPushArray(arena, T, n, nz)                                   \
    (T *)BecolArenaPush(arena, sizeof(T) * (n), (bool)nz)

/*
void *BecolMalloc(int size);
void BecolFree(void *mem);
char *BecolStrMalloc(char *str); // mallocs and strcpys a string so
                                 // that we can make sure a string
                                 // exists later
void BecolFreeAll();             // used for program cleanup. Really shouldn't
                                 // do anything but if something wasn't freed
                                 // then this would free it
*/

#endif
