#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

enum TokenType {
    INTEGER,
    FLOATING,
    PLUS,
    MINUS,
    MULTIPL,
    DIVISION,
    LEFT_PAR,
    RIGHT_PAR,
    EOI // end of input
};

typedef struct {
    int type;
    char *value;
} Token;

typedef struct {
    int pos;
    size_t input_len;
    char *input;
} Lexer;

Lexer *init_lexer(char *input);
Token *next_token(Lexer *lexer);
void free_token(Token *token);

#endif // LEXER_H
