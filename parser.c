#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"

static AST *parse_term(Parser *parser);
static AST *parse_factor(Parser *parser);
static AST *parse_unary(Parser *parser);
static AST *parse_primary(Parser *parser);

static void advance(Parser *parser);
static bool match(Parser *parser, int expected_type);

static void parser_error(Parser *parser, char *msg);
static void print_ast_rec(AST *node, int indent);

Parser *init_parser(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->prev = NULL;
    parser->cur = next_token(lexer);
    return parser;
}

AST *parser_parse(Parser *parser) {
    AST *ast = parse_term(parser);
    if (parser->cur->type != EOI) {
        parser_error(parser, "Expected end of input");
    }
    free_token(parser->cur);
    return ast;
}

static AST *parse_term(Parser *parser) {
    AST *expr = parse_factor(parser);
    while (match(parser, MINUS) || match(parser, PLUS)) {
        AST *term = malloc(sizeof(AST));
        term->left = expr;
        term->value = parser->prev;
        term->right = parse_factor(parser);
        term->type = BINARY;
        expr = term;
    }
    return expr;
}

static AST *parse_factor(Parser *parser) {
    AST *expr = parse_unary(parser);
    while (match(parser, MULTIPL) || match(parser, DIVISION)) {
        AST *factor = malloc(sizeof(AST));
        factor->left = expr;
        factor->value = parser->prev;
        factor->right = parse_unary(parser);
        factor->type = BINARY;
        expr = factor;
    }
    return expr;
}

static AST *parse_unary(Parser *parser) {
    if (match(parser, MINUS) || match(parser, PLUS)) {
        AST *unary = malloc(sizeof(AST));
        unary->left = NULL;
        unary->value = parser->prev;
        unary->right = parse_unary(parser);
        unary->type = UNARY;
        return unary;
    }
    return parse_primary(parser);
}

static AST *parse_primary(Parser *parser) {
    if (match(parser, INTEGER)) {
        AST *primary = malloc(sizeof(AST));
        primary->left = NULL;
        primary->right = NULL;
        primary->type = NUMBER;
        primary->value = parser->prev;
        return primary;
    }
    else if (match(parser, FLOATING)) {
        AST *primary = malloc(sizeof(AST));
        primary->left = NULL;
        primary->right = NULL;
        primary->type = FLOATING_NUMBER;
        primary->value = parser->prev;
        return primary;
    }
    else if (match(parser, LEFT_PAR)) {
        AST *grouping = malloc(sizeof(AST));
        grouping->value = parser->prev;
        grouping->left = parse_term(parser);
        grouping->right = NULL;
        grouping->type = GROUPING;
        if (!match(parser, RIGHT_PAR)) {
            parser_error(parser, "Expected ')'");
        }
        return grouping;
    }
    parser_error(parser, "Expected expression");
    return NULL;
}

static bool match(Parser *parser, int expected_type) {
    if (parser->cur->type == expected_type) {
        advance(parser);
        return true;
    }
    return false;
}

static void advance(Parser *parser) {
    parser->prev = parser->cur;
    parser->cur = next_token(parser->lexer);
}

void print_ast(AST *ast) {
    if (ast == NULL) return;
    print_ast_rec(ast, 0);
}

static void print_ast_rec(AST *node, int indent) {
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    if (indent != 0)
        printf("|_");
    printf("%s\n", node->value->value);
    print_ast_rec(node->left, indent + 2);
    print_ast_rec(node->right, indent + 2);
}

static void parser_error(Parser *parser, char *msg) {
    fprintf(stderr, "%s on pos %d\n", msg, parser->lexer->pos);
    exit(1);
}

void free_ast(AST *ast) {
    if (ast == NULL) return;
    free_token(ast->value);
    free_ast(ast->left);
    free_ast(ast->right);
    free(ast);
}
