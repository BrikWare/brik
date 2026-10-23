#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void run_hash_map_tests(void);
void run_arena_alloc_tests(void);

int main(void) {
    srand(time(NULL));

    run_arena_alloc_tests();
    run_hash_map_tests();

    fprintf(stdout, "SUCCESS: All tests passed!\n");
    return 0;
}