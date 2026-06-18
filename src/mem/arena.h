#ifndef BRIK_ARENA_H
#define BRIK_ARENA_H

#define ARENA_CHUNK_SIZE (4 * 1024 * 1024) // 4 MB

typedef struct Arena {
    char* memory;
    size_t offset;
    size_t capacity;
    struct Arena* next;
} Arena;

Arena* arena_create(size_t capacity);
void* arena_alloc(Arena* arena, size_t size);

#endif