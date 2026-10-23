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
    arena->current = arena;
    return arena;
}

void* arena_alloc(Arena* arena, const size_t size) {
    if (arena == NULL) return NULL;
    Arena* root = arena;
    Arena* current = arena->current;

    // if the provided allocation size is bigger than the current chunk's capacity we need to create a new chunk
    if (current->offset + size > current->capacity) {
        current->next = arena_create(size > ARENA_CHUNK_SIZE ? size : ARENA_CHUNK_SIZE);
        if (current->next == NULL) return NULL;

        root->current = current->next;
        current = current->next;
    }

    void* ptr = current->memory + current->offset;
    current->offset += size;
    return ptr;
}

void arena_reset(Arena* arena) {
    arena->offset = 0;
    arena->current = arena;
    Arena* current_extra_chunk = arena->next;
    arena->next = NULL;

    while (current_extra_chunk != NULL) {
        Arena* next_extra_chunk = current_extra_chunk->next;
        free(current_extra_chunk->memory);
        free(current_extra_chunk);
        current_extra_chunk = next_extra_chunk;
    }
}

void arena_destroy(Arena* arena) {
    arena_reset(arena);

    free(arena->memory);
    free(arena);
}