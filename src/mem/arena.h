#ifndef BRIK_ARENA_H
#define BRIK_ARENA_H

#define ARENA_CHUNK_SIZE (4 * 1024 * 1024) // 4 MB
#define DEFAULT_ALLOC_ALIGNMENT 16

typedef struct Arena {
    char* memory;
    size_t offset; // must be 8-byte aligned to
    size_t capacity;
    struct Arena* next;
    struct Arena* current; // keep track of arena tail chunk for O(1) allocations (basically only ever used by the arena root chunk)
} Arena;

Arena* arena_create(size_t capacity);
void* arena_alloc(Arena* arena, size_t size);

#endif