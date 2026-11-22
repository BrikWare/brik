#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"

char* read_file(const char* filepath) {
    if (filepath == NULL) {
        exit(1);
    }

    // open file
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        fprintf(stderr, "FATAL: could not open file \"%s\"\n", filepath);
        exit(74);
    }

    // get the file size in bytes
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file); // go back to the beginning of the file

    // allocate memory for the whole file + 1 for the null terminator char so the lexer knows when to stop
    char* buffer = (char*)malloc(file_size+1);
    if (buffer == NULL) {
        fprintf(stderr, "FATAL: not enough memory to read \"%s\"\n", filepath);
        fclose(file);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "FATAL: could not read entire file \"%s\".\n", filepath);
        free(buffer);
        fclose(file);
        exit(74);
    }

    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]){
    if (argc == 1) {
        fprintf(stderr, "no file provided\n");
        fprintf(stdout, "usage: brik <path_to_file.bk>\n");
        return 64;
    }
    if (argc > 2) {
        fprintf(stdout, "too many arguments\nusage: brik <path_to_file.bk>\n");
        return 64;
    }

    char* buffer = read_file(argv[1]);

    Arena* arena = arena_create(ARENA_CHUNK_SIZE);

    Lexer lex = lexer_create(buffer);

    Parser parser = parser_create(arena);

    Token token = lexer_get_next_token(&lex);
    while (token.type != TOKEN_EOF) {
        parser_add_token(&parser, token);
        printf("%s \t\t(%d, %zu)\n", token_type_name(token.type), token.line, token.length);
        token = lexer_get_next_token(&lex);
    }

    free(buffer);
    arena_destroy(arena);
    return 0;
}
