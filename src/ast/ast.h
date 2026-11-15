#ifndef BRIK_AST_H
#define BRIK_AST_H

#include <stdint.h>
#include <stddef.h>

#include "lexer/lexer.h"

typedef enum { NODE_BINARY, NODE_UNARY, NODE_LITERAL, NODE_IDENTIFIER, } NodeKind ;
typedef enum { LITERAL_INTEGER, LITERAL_FLOAT, LITERAL_STRING } LiteralKind;

typedef struct Node Node;
struct Node {
    int line;
    NodeKind kind;
    union {
        struct { TokenType op; Node* left; Node* right; } binary;
        struct { TokenType op; Node* operand; } unary;
        struct { LiteralKind kind;
            union { int64_t i; double f; struct {const char* start; size_t len;} s; };
        } literal;
        struct { const char* start; size_t len; } identifier;
    };
};

void dump_node(const Node* node, const int depth);

#endif //BRIK_AST_H
