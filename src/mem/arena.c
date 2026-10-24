#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
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


/* Allocate memory with alignment from the provided arena */
void* arena_alloc_aligned(Arena* arena, const size_t size, const size_t alignment) {
    if (arena == NULL || size == 0) return NULL;
    if ((alignment & (alignment - 1)) != 0) return NULL;

    Arena* root = arena; // keep track of the root chunk to later update its current chunk tracker
    Arena* current = arena->current;

    uintptr_t current_addr = (uintptr_t)(current->memory + current->offset);
    uintptr_t aligned_addr = (current_addr + (alignment-1)) & ~(uintptr_t)(alignment-1);
    size_t padding = (size_t)(aligned_addr - current_addr);

    // if the provided allocation size is bigger than the current chunk's capacity we need to create a new chunk
    if (current->offset+size+padding > current->capacity) {
        const size_t needed = size + alignment;
        const size_t chunk_cap = needed > ARENA_CHUNK_SIZE ? needed : ARENA_CHUNK_SIZE;

        current->next = arena_create(chunk_cap);
        if (current->next == NULL) return NULL;

        root->current = current->next; // update root chunk's current chunk tracker
        current = current->next; // update current chunk to the newly created one

        // recompute padding on the new chunk
        current_addr = (uintptr_t)(current->memory + current->offset);
        aligned_addr = (current_addr + (alignment - 1)) & ~(uintptr_t)(alignment - 1);
        padding = (size_t)(aligned_addr - current_addr);
    }

    current->offset += padding + size; // update offset
    return (void*)aligned_addr;
}


/* Allocate memory from the provided arena */
void* arena_alloc(Arena* arena, const size_t size) {
    return arena_alloc_aligned(arena, size, DEFAULT_ALLOC_ALIGNMENT);
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