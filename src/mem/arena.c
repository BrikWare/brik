#include <stdlib.h>
#include <stddef.h>
#include "arena.h"

Arena* arena_create(const size_t capacity) {
    Arena* arena = malloc(sizeof(Arena));
    if (arena == NULL) return NULL;

    arena->memory = malloc(capacity);
    if (arena->memory == NULL) {
        free(arena); // prevent leak
        return NULL;
    }

    arena->offset = 0;
    arena->capacity = capacity;
    arena->next = NULL;
    return arena;
}

void* arena_alloc(Arena* arena, const size_t size) {
    if (arena == NULL) return NULL;

    // if the provided allocation size is bigger than the current chunk's capacity we need to create a new chunk
    if (arena->offset + size > arena->capacity) {
        if (arena->next == NULL) { // if we are at the tail of the arena
            // if the demanded size is bigger than the default arena chunk size, allocate a chunk big enough to fit it
            // otherwise, allocate a default sized chunk
            arena->next = arena_create(size > ARENA_CHUNK_SIZE ? size : ARENA_CHUNK_SIZE);
            if (arena->next == NULL) return NULL;
        }

        return arena_alloc(arena->next, size);
    }

    void* ptr = arena->memory + arena->offset;
    arena->offset += size;
    return ptr;
}

void arena_reset(Arena* arena) {
    arena->offset = 0;
    Arena* current_extra_chunk = arena->next;
    arena->next = NULL;

    while (current_extra_chunk != NULL) {
        Arena* next_extra_chunk = current_extra_chunk->next;
        free(current_extra_chunk->memory);
        free(current_extra_chunk);
        current_extra_chunk = next_extra_chunk;
    }
}