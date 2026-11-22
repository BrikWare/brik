#include "parser.h"

#include <string.h>

Parser parser_create(Arena* arena) {
    Parser parser;
    parser.arena = arena;
    parser.tokens = arena_alloc(arena, sizeof(Token)*INITIAL_TOKEN_ALLOC_LENGTH);
    parser.allocated_length = INITIAL_TOKEN_ALLOC_LENGTH;
    parser.cursor = 0;
    parser.length = 0;

    return parser;
}

static void parser_grow(Parser* parser) {
    if (parser == NULL) return;

    const size_t new_capacity = parser->allocated_length * 2;
    Token* new_tokens = arena_alloc(parser->arena, sizeof(Token) * new_capacity);
    memcpy(new_tokens, parser->tokens, sizeof(Token) * parser->length);
    parser->tokens = new_tokens;
    parser->allocated_length = new_capacity;
}

void parser_add_token(Parser* parser, const Token token) {
    if (parser->length == parser->allocated_length) parser_grow(parser);

    parser->tokens[parser->length++] = token;
}