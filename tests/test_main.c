#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"

void run_hash_map_tests(void);
void run_arena_alloc_tests(void);
void run_ast_tests(void);

int main(void) {
    srand(time(NULL));

    run_arena_alloc_tests();
    run_hash_map_tests();
    run_ast_tests();

    fprintf(stdout, GREEN "SUCCESS: All tests passed!\n");
    return 0;
}