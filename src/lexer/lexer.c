#include <string.h>
#include "lexer.h"

static char advance(Lexer* lex);
static char look_ahead(const Lexer* lex);
static void skip_whitespace_and_comments(Lexer* lex);
static int is_at_end(const Lexer* lex);
static int is_digit(const char c);
static int is_alpha(const char c);
static int is_alphanumeric(const char c);
static Token make_token(const Lexer* lex, const TokenType type);
static int check_keyword(const char* start, const char* expected, size_t length);
static TokenType get_identifier_type(const Lexer* lex);
static Token sweep_string(Lexer* lex, const char quote_char, const TokenType token_type);
static Token sweep_identifier_or_keyword(Lexer* lex);
static Token sweep_number(Lexer* lex);

Token lexer_get_next_token(Lexer* lex) {
    skip_whitespace_and_comments(lex);

    lex->start_position = lex->position;

    if (is_at_end(lex)) return make_token(lex, TOKEN_EOF);

    const char c = lex->current_char;

    switch (c) {
        case '+': advance(lex); return make_token(lex, TOKEN_PLUS);
        case '*': advance(lex); return make_token(lex, TOKEN_MULT);
        case '/': advance(lex); return make_token(lex, TOKEN_DIV);
        case ',': advance(lex); return make_token(lex, TOKEN_COMMA);
        case '{': advance(lex); return make_token(lex, TOKEN_BRACES_LEFT);
        case '}': advance(lex); return make_token(lex, TOKEN_BRACES_RIGHT);
        case '[': advance(lex); return make_token(lex, TOKEN_BRACKET_LEFT);
        case ']': advance(lex); return make_token(lex, TOKEN_BRACKET_RIGHT);
        case '(': advance(lex); return make_token(lex, TOKEN_PARENTHESIS_LEFT);
        case ')': advance(lex); return make_token(lex, TOKEN_PARENTHESIS_RIGHT);
        case ':': advance(lex); return make_token(lex, TOKEN_COLON);
        case ';': advance(lex); return make_token(lex, TOKEN_SEMICOLON);
        case '.': advance(lex); return make_token(lex, TOKEN_DOT);
        case '@': advance(lex); return make_token(lex, TOKEN_AT);
        case '&': advance(lex); return make_token(lex, TOKEN_AMPERSAND);

        case '!':
            if (look_ahead(lex) == '='){ advance(lex); advance(lex); return make_token(lex, TOKEN_NOT_EQUAL); }
            advance(lex); return make_token(lex, TOKEN_BANG);
        case '=':
            if (look_ahead(lex) == '='){ advance(lex); advance(lex); return make_token(lex, TOKEN_EQUAL_EQUAL); }
            advance(lex); return make_token(lex, TOKEN_EQUAL);

        case '<':
            if (look_ahead(lex) == '='){ advance(lex); advance(lex); return make_token(lex, TOKEN_LESS_THAN_OR_EQUAL); }
            advance(lex); return make_token(lex, TOKEN_LESS_THAN);

        case '>':
            if (look_ahead(lex) == '='){ advance(lex); advance(lex); return make_token(lex, TOKEN_GREATER_THAN_OR_EQUAL); }
            advance(lex); return make_token(lex, TOKEN_GREATER_THAN);

        case '-':
            if (look_ahead(lex) == '>'){ advance(lex); advance(lex); return make_token(lex, TOKEN_ARROW); }

            advance(lex); return make_token(lex, TOKEN_MINUS);

        case '"':  return sweep_string(lex, '"', TOKEN_STRING);
        case '\'': return sweep_string(lex, '\'', TOKEN_CHAR);


        default:
            if (is_alpha(c)) { return sweep_identifier_or_keyword(lex); }
            if (is_digit(c)) { return sweep_number(lex); }

            advance(lex); return make_token(lex, TOKEN_ILLEGAL);
    }
}

// -- cursor --
static char advance(Lexer* lex) {
    if (lex->current_char == '\0') { return '\0'; }

    return lex->current_char = lex->source[++lex->position];
}

static char look_ahead(const Lexer* lex) {
    if (lex->current_char == '\0') { return '\0'; }

    return lex->source[lex->position+1];
}

static Token make_token(const Lexer* lex, const TokenType type) {
    Token token;
    token.type = type;
    token.start = lex->source + lex->start_position;
    token.length = lex->position - lex->start_position;
    token.line = lex->current_line;

    return token;
}

// -- whitespaces + comments --
static void skip_whitespace_and_comments(Lexer* lex) {
    char next_token;
    while (1) {
        switch (lex->current_char) {
            case ' ':
            case '\r':
            case '\t':
                advance(lex);
                break;

            case '\n':
                lex->current_line++;
                advance(lex);
                lex->line_start = lex->position;
                break;

            case '/':
                next_token = look_ahead(lex);
                if (next_token == '/') { // single line comment, eat until newline or EOF
                    while (lex->current_char != '\n' && lex->current_char != '\0') {
                        advance(lex);
                    }
                } else if (next_token == '*') { // multi line comment
                    advance(lex); // consume the '/'
                    advance(lex); // consume the '*'

                    while (lex->current_char != '\0') {
                        if (lex->current_char == '\n') {
                            lex->current_line++;
                            advance(lex);
                            lex->line_start = lex->position;
                        } else if (lex->current_char == '*' && look_ahead(lex) == '/') {
                            advance(lex); // consume the '*'
                            advance(lex); // consume the '/'
                            break;
                        } else {
                            advance(lex);
                        }
                    }
                } else {
                    return; // the current token is not a comment and must be processed, so just return
                }
                break;
            default:
                return;
        }
    }
}

// -- chars --
static int is_at_end(const Lexer* lex) {
    return lex->current_char=='\0';
}

static int is_digit(const char c) {
    return c >= '0' && c <= '9';
}

static int is_alpha(const char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static int is_alphanumeric(const char c) {
    return is_alpha(c) || is_digit(c);
}

// -- keywords --

// fast helper to compare a raw memory slice against a literal string
static int check_keyword(const char* start, const char* expected, size_t length) {
    return memcmp(start, expected, length) == 0;
}

static TokenType get_identifier_type(const Lexer* lex) {
    const size_t length = lex->position - lex->start_position;
    const char* start = lex->source + lex->start_position;
    switch (length) {
        case 2:
            if (start[0]=='i' && start[1]=='f'){ return TOKEN_IF; }
            if (start[0]=='f' && start[1]=='n'){ return TOKEN_FN; }
            break;
        case 3:
            if (start[0]=='l' && start[1]=='e' && start[2]=='t'){ return TOKEN_LET; }
            if (start[0]=='f' && start[1]=='o' && start[2]=='r'){ return TOKEN_FOR; }
            break;
        case 4:
            if (check_keyword(start, "true", 4)){ return TOKEN_TRUE; }
            if (check_keyword(start, "else", 4)){ return TOKEN_ELSE; }
            if (check_keyword(start, "impl", 4)){ return TOKEN_IMPL; }
            break;
        case 5:
            if (check_keyword(start, "false", 5)){ return TOKEN_FALSE; }
            if (check_keyword(start, "while", 5)){ return TOKEN_WHILE; }
            break;
        case 6:
            if (check_keyword(start, "struct", 6)){ return TOKEN_STRUCT; }
            if (check_keyword(start, "return", 6)){ return TOKEN_RETURN; }
        default:
            break;
    }
    return TOKEN_IDENTIFIER;
}

// -- sweeps --
static Token sweep_string(Lexer* lex, const char quote_char, const TokenType token_type) {
    advance(lex); // step past the quote to read the body

    while (lex->current_char != quote_char && lex->current_char != '\0') {
        if (lex->current_char == '\n') {
            lex->current_line++;
            lex->line_start = lex->position;
        }

        // handle escape sequences like \" or \'
        if (lex->current_char == '\\') {
            advance(lex); // skip '\'
            if (lex->current_char == '\0') break;
        }

        advance(lex);
    }

    if (lex->current_char == '\0') {
        // unterminated string error
        return make_token(lex, TOKEN_ILLEGAL);
    }

    // consume the closing quote
    advance(lex);

    return make_token(lex, token_type);
}

static Token sweep_identifier_or_keyword(Lexer* lex) {
    while (is_alphanumeric(lex->current_char)) {
        advance(lex);
    };

    const TokenType type = get_identifier_type(lex);
    return make_token(lex, type);
}

static Token sweep_number(Lexer* lex) {
    while (is_digit(lex->current_char)) {
        advance(lex);
    }

    if (lex->current_char == '.' && is_digit(look_ahead(lex))) {
        advance(lex); // consume '.'
        while (is_digit(lex->current_char)) {
            advance(lex);
        }
    }

    return make_token(lex, TOKEN_NUMBER);
}
