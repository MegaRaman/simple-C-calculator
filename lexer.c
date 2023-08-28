#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"

#define MAX_TOKENS 10
#define MAX_INT_LEN 10

static Token *lex_num(Lexer *lexer);
static Token *lex_op(Lexer *lexer);
static Token *lex_par(Lexer *lexer);
static Token *lex_eoi(Lexer *lexer);
static void lex_whitespace(Lexer *lexer);
static bool lex_advance(Lexer *lexer);
static void lex_error(Lexer *lexer, char *msg);

Lexer *init_lexer(char *input) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->pos = 0;
    lexer->input = input;
    lexer->input_len = strlen(input);
    return lexer;
}

Token *next_token(Lexer *lexer) {
    lex_whitespace(lexer);
    char cur_char = lexer->input[lexer->pos];
    if (cur_char == '\n') {
        return lex_eoi(lexer);
    }
    if (cur_char >= '0' && cur_char <='9') {
        return lex_num(lexer);
    }
    if (cur_char == '+' || cur_char == '-' || cur_char == '*'
            || cur_char == '/') {
        return lex_op(lexer);
    }
    if (cur_char == '(' || cur_char == ')') {
        return lex_par(lexer);
    }
    lex_error(lexer, "Invalid character");
    return NULL;
}

static Token *lex_num(Lexer *lexer) {
    Token *token = malloc(sizeof(Token));
    token->type = INTEGER;
    token->value = malloc(MAX_INT_LEN);
    char cur_char = lexer->input[lexer->pos];
    int pos = 0;
    while ((cur_char >= '0' && cur_char <= '9') || cur_char == '.') {
        if (cur_char == '.') {
            token->type = FLOATING;
        }
        token->value[pos] = cur_char;
        if (!lex_advance(lexer))
            break;
        cur_char = lexer->input[lexer->pos];
        pos++;
    }
    
    return token;
}

static Token *lex_op(Lexer *lexer) {
    Token *token = malloc(sizeof(Token));
    switch (lexer->input[lexer->pos])
    {
    case '+':
        token->type = PLUS;
        token->value = "+";
        break;
    case '-':
        token->type = MINUS;
        token->value = "-";
        break;
    case '*':
        token->type = MULTIPL;
        token->value = "*";
        break;
    case '/':
        token->type = DIVISION;
        token->value = "/";
        break;
    default:
        break;
    }
    lex_advance(lexer);
    return token;
}

static Token *lex_eoi(Lexer *lexer) {
    Token *token = malloc(sizeof(Token));
    token->type = EOI;
    token->value = NULL;
    return token;
}

static Token *lex_par(Lexer *lexer) {
    Token *token = malloc(sizeof(Token));
    if (lexer->input[lexer->pos] == '(') {
        token->type = LEFT_PAR;
        token->value = "(";
    }
    else {
        token->type = RIGHT_PAR;
        token->value = ")";
    }
    lex_advance(lexer);
    return token;
}

static void lex_whitespace(Lexer *lexer) {
    while ((lexer->input[lexer->pos] == ' ' || lexer->input[lexer->pos] == '\t')
         && lex_advance(lexer));
}

static bool lex_advance(Lexer *lexer) {
    if (lexer->pos < lexer->input_len) {
        ++lexer->pos;
        return true;
    }
    return false;
}

static void lex_error(Lexer *lexer, char *msg) {
    fprintf(stderr, "%s on pos %d\n", msg, lexer->pos + 1);
    exit(-1);
}

void free_token(Token *token) {
    if (token->type == INTEGER) {
        free(token->value);
    }
    free(token);
}
