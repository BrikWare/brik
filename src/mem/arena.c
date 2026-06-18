#include <stdlib.h>
#include <stddef.h>
#include "arena.h"

Arena* arena_create(size_t capacity) {
    Arena* arena = malloc(sizeof(Arena));
    arena->memory = malloc(capacity);
    if (arena->memory == NULL) return NULL;
    arena->offset = 0;
    arena->capacity = capacity;
    arena->next = NULL;
    return arena;
}

void* arena_alloc(Arena* arena, size_t size) {
    if (arena == NULL) return NULL;

    if (arena->offset + size > arena->capacity) {
        while (arena->next == NULL) {
            arena->next = arena_create(ARENA_CHUNK_SIZE);
        }
        return arena_alloc(arena->next, size);
    }

    void* ptr = arena->memory + arena->offset;
    arena->offset += size;
    return ptr;
}