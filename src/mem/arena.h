#ifndef BRIK_ARENA_H
#define BRIK_ARENA_H

#include <stdalign.h>

#define ARENA_CHUNK_SIZE (4 * 1024 * 1024) // 4 MB
#define DEFAULT_ALLOC_ALIGNMENT alignof(max_align_t)

typedef struct Arena {
    char* memory;
    size_t offset; // must be 8-byte aligned to
    size_t capacity;
    struct Arena* next;
    struct Arena* current; // keep track of arena tail chunk for O(1) allocations (basically only ever used by the arena root chunk)
} Arena;

Arena* arena_create(size_t capacity);
void* arena_alloc(Arena* arena, size_t size);
void* arena_alloc_aligned(Arena* arena, size_t size, size_t alignment);
void arena_reset(Arena* arena);
void arena_destroy(Arena* arena);

#endif