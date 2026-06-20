#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/hash_map.h"

void generate_random_string(char* buffer, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (size_t i = 0; i < length; ++i) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    buffer[length] = '\0';
}

void insert_get(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    HashMap* hash_map = create_hash_map(arena, 8);

    char* key = "return";
    const uint32_t value = 5;

    hash_map_insert(hash_map, key, value);
    const MapResult result = hash_map_get(hash_map, key);

    assert(result.is_valid && result.value == value);
}

void get_non_existing_entry(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    const HashMap* hash_map = create_hash_map(arena, 8);

    const char* key = "non_existing_entry";
    const MapResult result = hash_map_get(hash_map, key);

    assert(result.is_valid == false);
}

void test_resizing_and_integrity(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    const size_t initial_map_capacity = 8;
    const size_t resized_map_capacity = initial_map_capacity * 2;
    HashMap* hash_map = create_hash_map(arena, initial_map_capacity);

    char* control_key = "control";
    const uint32_t control_value = 64;

    hash_map_insert(hash_map, control_key, control_value);

    // we only insert 4 entries because the map resizes when the capacity hits half
    // as to keep the collisions at a minimum, so when the entries count hits 4, the map
    // automatically resizes and we can check fro the new capacity value
    for (size_t i=0; i<initial_map_capacity/2; ++i) {
        size_t str_len = rand() % 100;
        char* str = arena_alloc(arena, str_len);
        generate_random_string(str, str_len);
        hash_map_insert(hash_map, str, str_len);
    }

    // ensure correct resizing
    assert(hash_map->capacity == resized_map_capacity);

    // ensure the entry we inserted before the resizing is still there
    const MapResult result = hash_map_get(hash_map, control_key);
    assert(result.is_valid && result.value == control_value);
}


void run_hash_map_tests(void) {
    insert_get();
    get_non_existing_entry();
    test_resizing_and_integrity();
}
