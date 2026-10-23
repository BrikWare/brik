#include <stdlib.h>
#include <stddef.h>
#include "arena.h"


/* Create Arena with the provided capacity */
Arena* arena_create(size_t capacity) {
    Arena* arena = malloc(sizeof(Arena));
    if (arena == NULL) return NULL;

    // align capacity
    capacity += (DEFAULT_ALLOC_ALIGNMENT-1);
    capacity &= (~(DEFAULT_ALLOC_ALIGNMENT-1));

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


/* Allocate memory from the provided arena */
void* arena_alloc(Arena* arena, size_t size) {
    if (arena == NULL) return NULL;
    Arena* root = arena; // keep track of the root chunk to later update its current chunk tracker
    Arena* current = arena->current;

    // align capacity
    size += (DEFAULT_ALLOC_ALIGNMENT-1);
    size &= (~(DEFAULT_ALLOC_ALIGNMENT-1));

    // if the provided allocation size is bigger than the current chunk's capacity we need to create a new chunk
    if (current->offset + size > current->capacity) {
        current->next = arena_create(size > ARENA_CHUNK_SIZE ? size : ARENA_CHUNK_SIZE);
        if (current->next == NULL) return NULL;

        root->current = current->next; // update root chunk's current chunk tracker
        current = current->next; // update current chunk to the newly created one
    }

    void* ptr = current->memory + current->offset; // compute mem pointer
    current->offset += size; // update offset
    return ptr;
}


/* Reset Arena to the first chunk */
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


/* Free entire Arena */
void arena_destroy(Arena* arena) {
    arena_reset(arena);

    free(arena->memory);
    free(arena);
}