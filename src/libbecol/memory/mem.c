#include "mem.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>

uint32_t BecolGetPageSize(void) {
    SYSTEM_INFO sysinfo = {0};
    GetSystemInfo(&sysinfo);

    return sysinfo.dwPageSize;
}

void *BecolReserveMemory(uint64_t size) {
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
}

void *BecolCommitMemory(void *ptr, uint64_t size) {
    void *ret = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
    return ret != NULL;
}

bool BecolDecommitMemory(void *ptr, uint64_t size) {
    return VirtualFree(ptr, size, MEM_DECOMMIT);
}

bool BecolReleaseMemory(void *ptr, uint64_t size) {
    return VirtualFree(ptr, size, MEM_RELEASE);
}

#elif defined(__linux__)

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <sys/mman.h>
#include <unistd.h>

uint32_t BecolGetPageSize(void) { return (uint32_t)sysconf(_SC_PAGESIZE); }

void *BecolReserveMemory(uint64_t size) {
    void *ret = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ret == MAP_FAILED) {
        fprintf(stderr, "Failed to mmap memory\n");
        return NULL;
    }

    return ret;
}

bool BecolCommitMemory(void *ptr, uint64_t size) {
    int32_t ret = mprotect(ptr, size, PROT_READ | PROT_WRITE);
    return ret == 0;
}

bool BecolDecommitMemory(void *ptr, uint64_t size) {
    int32_t ret = mprotect(ptr, size, PROT_NONE);
    if (ret != 0) {
        fprintf(stderr, "Failed to mprotect memory\n");
        return false;
    }
    ret = madvise(ptr, size, MADV_DONTNEED);
    return ret == 0;
}

bool BecolReleaseMemory(void *ptr, uint64_t size) {
    int32_t ret = munmap(ptr, size);
    return ret == 0;
}
#else
#error "MacOS is unsupported"
#endif

MemoryArena *BecolArenaCreate(uint64_t reserve_size, uint64_t commit_size) {
    uint32_t page_size = BecolGetPageSize();

    reserve_size = ALIGN_UP_TO_POW2(reserve_size, page_size);
    commit_size = ALIGN_UP_TO_POW2(commit_size, page_size);

    MemoryArena *arena = BecolReserveMemory(reserve_size);

    if (!BecolCommitMemory(arena, commit_size)) {
        fprintf(stderr, "Failed to commit memory\n");
        return NULL;
    }

    arena->reserved = reserve_size;
    arena->committed = commit_size;
    arena->position = ARENA_BASE_POS;
    arena->commit_position = commit_size;

    return arena;
}

void BecolArenaDestroy(MemoryArena *arena) {
    BecolReleaseMemory(arena, arena->reserved);
}

void *BecolArenaPush(MemoryArena *arena, uint64_t size, bool non_zero) {
    uint64_t pos_aligned = ALIGN_UP_TO_POW2(arena->position, ARENA_ALIGN);
    uint64_t new_pos = pos_aligned + size;

    if (new_pos > arena->reserved) {
        fprintf(stderr,
                "Failed to allocate memory, pos is larger than reserved\n");
        return NULL;
    }

    if (new_pos > arena->commit_position) {
        uint64_t new_commit_pos = new_pos;
        new_commit_pos += arena->committed - 1;
        new_commit_pos -= new_commit_pos % arena->committed;
        new_commit_pos = MIN(new_commit_pos, arena->reserved);

        uint8_t *mem = (uint8_t *)arena + arena->commit_position;
        uint64_t commit_size = new_commit_pos - arena->commit_position;

        if (!BecolCommitMemory(mem, commit_size)) {
            fprintf(stderr, "Failed to commit memory\n");
            return NULL;
        }

        arena->commit_position = new_commit_pos;
    }

    arena->position = new_pos;

    uint8_t *out = (uint8_t *)arena + pos_aligned;
    if (!non_zero)
        memset(out, 0, size);

    return (void *)out;
}

void BecolArenaPop(MemoryArena *arena, uint64_t size) {
    size = ALIGN_UP_TO_POW2(size, arena->position - ARENA_BASE_POS);
    arena->position -= size;
}

void BecolArenaPopTo(MemoryArena *arena, uint64_t pos) {
    uint64_t size = pos < arena->position ? arena->position - pos : 0;
    BecolArenaPop(arena, size);
}

void BecolArenaClear(MemoryArena *arena) {
    BecolArenaPopTo(arena, ARENA_BASE_POS);
}

TemporaryArena BecolTemporaryArenaBegin(MemoryArena *arena) {
    return (TemporaryArena){.arena = arena, .start_pos = arena->position};
}

void BecolTemporaryArenaEnd(TemporaryArena temp) {
    BecolArenaPopTo(temp.arena, temp.start_pos);
}

static __thread MemoryArena *__scratch_arenas[2] = {NULL, NULL};

TemporaryArena BecolArenaScratchGet(MemoryArena **conflicts,
                                    uint32_t num_conflicts) {
    int32_t scratch_id = -1;

    for (int32_t i = 0; i < 2; i++) {
        bool found = false;

        for (uint32_t j = 0; j < num_conflicts; j++) {
            if (conflicts[j] == __scratch_arenas[i]) {
                found = true;
                break;
            }
        }

        if (!found) {
            scratch_id = i;
            break;
        }
    }

    if (scratch_id == -1) {
        return (TemporaryArena){0};
    }

    MemoryArena **selected = &__scratch_arenas[scratch_id];

    if (*selected == NULL) {
        *selected = BecolArenaCreate(MiB(64), MiB(1));
    }

    return BecolTemporaryArenaBegin(*selected);
}

void BecolArenaScratchRelease(TemporaryArena scratch) {
    BecolTemporaryArenaEnd(scratch);
}
