#ifndef BRIK_LEXER_H
#define BRIK_LEXER_H

#include <stddef.h>

typedef enum {
    TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER,

    // logic
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV,
    TOKEN_EQUAL_EQUAL, TOKEN_BANG, TOKEN_GREATER_THAN, TOKEN_GREATER_THAN_OR_EQUAL, TOKEN_LESS_THAN, TOKEN_LESS_THAN_OR_EQUAL,

    // assignment
    TOKEN_EQUAL,

    // keywords
    TOKEN_LET, TOKEN_FN,
    TOKEN_STRUCT, TOKEN_IMPL,
    TOKEN_IF, TOKEN_ELSE,
    TOKEN_FOR, TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_TRUE, TOKEN_FALSE,

    // type system
    TOKEN_COLON,

    // pointers
    TOKEN_ARROW, TOKEN_AMPERSAND,

    // typestate trigger
    TOKEN_DOT,

    // comptime trigger
    TOKEN_AT,

    // structural boundaries
    TOKEN_PARENTHESIS_LEFT, TOKEN_PARENTHESIS_RIGHT,
    TOKEN_BRACKET_LEFT, TOKEN_BRACKET_RIGHT,
    TOKEN_BRACES_LEFT, TOKEN_BRACES_RIGHT,
    TOKEN_SEMICOLON, TOKEN_COMMA,


    // system
    TOKEN_ILLEGAL, TOKEN_EOF,
} TokenType;

typedef struct {
    const char* start;
    size_t length;
    TokenType type;
    int line;
} Token;

typedef struct {
    char* source; // stream to compile
    size_t position; // absolute byte offset of the reading head
    size_t start_position; // absolute byte offset of the current token's start
    size_t line_start; // absolute byte offset of the last '\n'
    char current_char; // byte at the index
    int current_line; // line tracking for reporting errors
} Lexer;

Token lexer_get_next_token(Lexer* lex);

#endif //BRIK_LEXER_H