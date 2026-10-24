#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mem/arena.h"


static int is_aligned(const void* ptr, size_t alignment) {
    return ((uintptr_t)ptr & (alignment - 1)) == 0;
}


void allocate_bigger_than_chunk(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    assert(arena != NULL);

    arena_alloc(arena, ARENA_CHUNK_SIZE*2);
    assert(arena != NULL);
    assert(arena->next != NULL);

    arena_destroy(arena);
    printf("PASS: allocate_bigger_than_chunk\n");
}


void test_arena_4096_alignment(void) {
    const size_t PAGE_SIZE = 4096;
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);
    assert(arena != NULL);

    // initial aligned allocation on a fresh chunk
    void* p1 = arena_alloc_aligned(arena, 64, PAGE_SIZE);
    assert(p1 != NULL);
    assert(is_aligned(p1, PAGE_SIZE));

    // write to memory to confirm it is safe to access
    *(volatile uint64_t*)p1 = 0xDEADBEEFCAFEBABE;
    assert(*(volatile uint64_t*)p1 == 0xDEADBEEFCAFEBABE);

    // introduce offset misalignment
    void* unaligned_byte = arena_alloc(arena, 3);
    assert(unaligned_byte != NULL);

    // request another 4096-aligned page, arena must skip padding
    void* p2 = arena_alloc_aligned(arena, 128, PAGE_SIZE);
    assert(p2 != NULL);
    assert(is_aligned(p2, PAGE_SIZE));
    assert((uintptr_t)p2 > (uintptr_t)unaligned_byte);

    // force a new chunk creation with large page-aligned allocation
    size_t big_request = ARENA_CHUNK_SIZE - (PAGE_SIZE * 2);
    void* big_block = arena_alloc(arena, big_request);
    assert(big_block != NULL);

    // this allocation will exceed the chunk capacity and create a new chunk
    void* p3 = arena_alloc_aligned(arena, PAGE_SIZE, PAGE_SIZE);
    assert(p3 != NULL);
    assert(is_aligned(p3, PAGE_SIZE));

    // verify root->current was updated properly
    assert(arena->current != arena);

    // clean up
    arena_destroy(arena);
    printf("PASS: test_arena_4096_alignment\n");
}


void run_arena_alloc_tests(void) {
    printf("\n----------Arena Allocation Tests----------\n");
    allocate_bigger_than_chunk();
    test_arena_4096_alignment();
    printf("-----------------------------------------\n");
}