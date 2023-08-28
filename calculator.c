#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast_evaluator.h"

#define MAX_INPUT 128

int main(int argc, char **argv) {
    printf("Enter an expression to evaluate: \n");
    
    char *input = malloc(MAX_INPUT);
    fgets(input, MAX_INPUT, stdin);

    Lexer *lex = init_lexer(input);
    Parser *parser = init_parser(lex);
    AST *expr_ast = parser_parse(parser);

    // print_ast(expr_ast);
    evaluate(expr_ast);
    
    free_ast(expr_ast);
    free(parser);
    free(lex);
    free(input);
    return 0;
}
