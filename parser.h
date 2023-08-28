#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum NodeType {
    UNARY,
    BINARY,
    NUMBER,
    FLOATING_NUMBER,
    GROUPING
};

typedef struct AST {
    int type;
    Token *value;
    struct AST *left;
    struct AST *right;
} AST;

typedef struct {
    Token *prev;
    Token *cur;
    Lexer *lexer;
} Parser;

Parser *init_parser(Lexer *lexer);
AST *parser_parse(Parser *parser);
void parser_advance(Parser *parser);
void print_ast(AST *ast);
void free_ast(AST *ast);

#endif // PARSER_H
