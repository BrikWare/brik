#include "utils/hash_map.h"
#include <assert.h>

void insert_get(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    HashMap* hash_map = create_hash_map(arena, 8);

    char* key = "return";
    const uint32_t value = 5;

    hash_map_insert(hash_map, key, value);
    const MapResult result = hash_map_get(hash_map, key);

    assert(result.is_valid && result.value == value);
}

void run_hash_map_tests(void) {
    insert_get();
}
