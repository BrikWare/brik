#ifndef BRIK_HASH_MAP_H
#define BRIK_HASH_MAP_H

#include <stddef.h>
#include <stdint.h>
#include "mem/arena.h"

#define HASH_MAP_INITIAL_SIZE 64

typedef struct HashEntry {
    char* key;
    uint32_t value;
} HashEntry;

typedef struct HashMap {
    HashEntry* entries;
    size_t capacity;
    size_t count;
    Arena* arena;
} HashMap;

HashMap* create_hash_map(Arena* arena, size_t capacity);
uint32_t hash_str(const char* str);
void insert(HashMap* hash_map, char* str, const uint32_t value);
void resize(const HashMap* old_hash_map);

#endif //BRIK_HASH_MAP_H