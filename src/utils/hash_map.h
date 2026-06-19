#ifndef BRIK_HASH_MAP_H
#define BRIK_HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "mem/arena.h"

#define HASH_MAP_DEFAULT_INITIAL_SIZE 64

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

typedef struct MapResult {
    bool is_valid;
    uint32_t value;
} MapResult;

HashMap* create_hash_map(Arena* arena, size_t capacity);
uint32_t hash_str(const char* str);
void hash_map_insert(HashMap* hash_map, char* str, uint32_t value);
void hash_map_resize(HashMap* hash_map);
MapResult hash_map_get(const HashMap* hash_map, const char* str);

#endif //BRIK_HASH_MAP_H