#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem/arena.h"


void allocate_bigger_than_chunk(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    assert(arena != NULL);

    arena_alloc(arena, ARENA_CHUNK_SIZE*2);
    assert(arena != NULL);
}


void run_arena_alloc_tests(void) {
    allocate_bigger_than_chunk();
}