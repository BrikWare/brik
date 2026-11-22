#include <stdio.h>
#include <assert.h>
#include "lexer/lexer.h"

static Lexer setup(char* src) {
    Lexer lex;
    lex.source = src;
    lex.position = 0;
    lex.start_position = 0;
    lex.line_start = 0;
    lex.current_char = lex.source[lex.position];
    lex.current_line = 1;

    return lex;
}

void test_multi_line_comment(void) {
    char* file =
        "let x = 1;\n"
        "/* line 2\n"
        "line 3\n"
        "line 4 */\n"
        "let y = 5\n";

    Lexer lex = setup(file);

    int lines[9] = {1,1,1,1,1,5,5,5,5};

    Token token = lexer_get_next_token(&lex);
    int i = 0;
    while (token.type != TOKEN_EOF) {
        assert(lines[i++] == token.line);
        token = lexer_get_next_token(&lex);
    }
    assert(i==9);

    printf("PASS: test_multi_line_comment\n");
}

void run_lexer_tests(void) {
    printf("\n----------Lexer Tests----------\n");
    test_multi_line_comment();
    printf("-------------------------------\n");
}