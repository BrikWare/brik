#include <stdio.h>
#include <inttypes.h>
#include "ast.h"

void dump_node(const Node* node, const int depth) {
    int tmp_depth = depth;
    while (tmp_depth) {
        printf(" ");
        tmp_depth--;
    }

    switch (node->kind) {
        case NODE_BINARY:
            printf("%s\n", token_type_name(node->binary.op));
            dump_node(node->binary.left, depth+1);
            dump_node(node->binary.right, depth+1);
            break;
        case NODE_UNARY:
            printf("%s\n", token_type_name(node->unary.op));
            dump_node(node->unary.operand, depth+1);
            break;
        case NODE_LITERAL:
            switch (node->literal.kind) {
                case LITERAL_INTEGER:
                    printf("%" PRId64 "\n", node->literal.i);
                    break;
                case LITERAL_FLOAT:
                    printf("%f\n", node->literal.f);
                    break;
                case LITERAL_STRING:
                    printf("%.*s\n", (int)node->literal.s.len, node->literal.s.start);
                    break;
            }
            break;
        case NODE_IDENTIFIER:
            printf("%.*s\n", (int)node->identifier.len, node->identifier.start);
    }
}
