#include "hash_map.h"

HashMap* create_hash_map(Arena* arena, const size_t capacity) {
    if (capacity <= 0) { // can't create empty hash_map
        return NULL;
    }

    // allocate hash map
    const size_t hash_map_size = sizeof(HashMap);
    HashMap* hash_map = arena_alloc(arena, hash_map_size);
    hash_map->capacity = capacity;
    hash_map->count = 0;
    hash_map->arena = arena;

    // allocate each entry in the hash map
    const size_t hash_entries_size = sizeof(HashEntry) * capacity;
    hash_map->entries = arena_alloc(arena, hash_entries_size);
    for (size_t i=0; i<capacity; ++i) {
        hash_map->entries[i].key = NULL;
    }

    return hash_map;
}

uint32_t hash_str(const char* str) {
    uint32_t hash = 2166136261u; // FNV offset basis
    for (int i=0; str[i]!='\0'; ++i) {
        hash = (hash ^ str[i]) * 16777619; // XOR + prime multiplication
    }
    return hash;
}

void resize(HashMap* hash_map) {
    size_t old_capacity = hash_map->capacity;
    size_t new_capacity = old_capacity * 2;
    HashEntry* old_entries = hash_map->entries;

    // allocate only the new entries array
    HashEntry* new_entries = arena_alloc(hash_map->arena, sizeof(HashEntry) * new_capacity);
    for (size_t i = 0; i < new_capacity; ++i) {
        new_entries[i].key = NULL;
    }

    hash_map->entries = new_entries;
    hash_map->capacity = new_capacity;
    hash_map->count = 0;

    // rehash the old entries into the new array
    for (size_t i = 0; i < old_capacity; ++i) {
        if (old_entries[i].key != NULL) {
            insert(hash_map, old_entries[i].key, old_entries[i].value);
        }
    }
}

void insert(HashMap* hash_map, char* str, const uint32_t value) {
    // if the hash map has reached half of its full size, resize with double the capacity
    if (hash_map->count >= hash_map->capacity/2) {
        resize(hash_map);
    }

    uint32_t hash = hash_str(str);
    hash %= hash_map->capacity;

    // find an empty entry by hashing and linear probing in case of collision
    while (1) {
        if (hash_map->entries[hash].key == NULL) {
            hash_map->entries[hash].key = str;
            hash_map->entries[hash].value = value;
            ++hash_map->count;
            break;
        }
        hash = (hash + 1) % hash_map->capacity; // increment the hash and modulo to loop around the hash_map
    }
}
