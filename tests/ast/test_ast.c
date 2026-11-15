#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "mem/arena.h"

static Node* new_node(Arena* arena, NodeKind kind) {
    Node* node = arena_alloc(arena, sizeof(Node));
    *node = (Node){ .kind = kind, .line = 1 };
    return node;
}

void test_simple_ast(void) {
    Arena* arena = arena_create(ARENA_CHUNK_SIZE);

    //test a simple ast with precedence: 2 + 3 * x

    Node* two = new_node(arena, NODE_LITERAL);
    two->literal.kind = LITERAL_INTEGER;
    two->literal.i = 2;

    Node* three = new_node(arena, NODE_LITERAL);
    three->literal.kind = LITERAL_INTEGER;
    three->literal.i = 3;

    Node* x = new_node(arena, NODE_IDENTIFIER);
    x->identifier.start = "x";
    x->identifier.len = 1;

    Node* mul = new_node(arena, NODE_BINARY);
    mul->binary.left = two;
    mul->binary.right = three;
    mul->binary.op = TOKEN_MULT;

    Node* plus = new_node(arena, NODE_BINARY);
    plus->binary.left = mul;
    plus->binary.right = x;
    plus->binary.op = TOKEN_PLUS;

    dump_node(plus, 0);

    assert(plus->kind == NODE_BINARY && plus->binary.op == TOKEN_PLUS);
    assert(plus->binary.left == mul);
    assert(mul->kind == NODE_BINARY && mul->binary.op == TOKEN_MULT);
    assert(mul->binary.left == two && mul->binary.left->literal.i == 2);
    assert(mul->binary.right == three && mul->binary.right->literal.i == 3);
    assert(plus->binary.right == x);
    assert(x->kind == NODE_IDENTIFIER && x->identifier.len == 1 && x->identifier.start[0] == 'x');

    arena_destroy(arena);
}

void run_ast_tests(void) {
    printf("\n----------AST Tests----------\n");
    test_simple_ast();
    printf("\n-----------------------------\n");
}