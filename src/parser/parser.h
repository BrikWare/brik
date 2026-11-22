#ifndef BRIK_PARSER_H
#define BRIK_PARSER_H

#define INITIAL_TOKEN_ALLOC_LENGTH 16

#include "lexer/lexer.h"
#include "mem/arena.h"

typedef struct {
    Arena* arena;
    Token* tokens; // array of lexed tokens
    size_t length; // number of effective lexed tokens in the array
    size_t allocated_length; // capacity of lexed tokens in current array (gets updated as it grows)
    size_t cursor; // current lexed token being read
} Parser;

Parser parser_create(Arena* arena);
void parser_add_token(Parser* parser, const Token token);

#endif //BRIK_PARSER_H
