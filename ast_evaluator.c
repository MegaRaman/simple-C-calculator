#include <stdlib.h>
#include "ast_evaluator.h"

static float evaluate_rec(AST *node);

void evaluate(AST *ast_root) {
    float result = evaluate_rec(ast_root);
    printf("Result of this arithmetic expression is %g\n", result);
}

static float evaluate_rec(AST *node) {
    int tmp;
    switch (node->type) {
    case NUMBER:
        return atoi(node->value->value);
    case FLOATING_NUMBER:
        return atof(node->value->value);
    case UNARY:
        if (node->value->type == PLUS) {
            return evaluate_rec(node->right);
        }
        if (node->value->type == MINUS) {
            return -evaluate_rec(node->right);
        }
    case BINARY:
        if (node->value->type == PLUS) {
            return evaluate_rec(node->left) + evaluate_rec(node->right); 
        }
        if (node->value->type == MINUS) {
            return evaluate_rec(node->left) - evaluate_rec(node->right); 
        }
        if (node->value->type == MULTIPL) {
            return evaluate_rec(node->left) * evaluate_rec(node->right);
        }
        if (node->value->type == DIVISION) {
            tmp = evaluate_rec(node->right);
            if (tmp == 0) {
                fprintf(stderr, "Error: division by zero\n");
                exit(-1);
            }
            return evaluate_rec(node->left) / tmp;
        }
    case GROUPING:
        return evaluate_rec(node->left);
    }
    return -1;
}
