/*
四则运算
G(M)
<expr> ::= <expr> + <term>
         | <expr> - <term>
         | <term>

<term> ::= <term> * <factor>
         | <term> / <factor>
         | <factor>

<factor> ::= ( <expr> )
           | Num
*/

/*
直接消除左递归

<expr> ::= <term> <expr_tail>
<expr_tail> ::= <empty>
              | + <term> <expr_tail>
              | - <term> <expr_tail>

<term> ::= <factor> <term_tail>
<term_tail> ::= <empty>
              | * <factor> <term_tail>
              | / <factor> <term_tail>

<factor> ::= ( <expr> )
           | Num
*/

/*
BNF表示法消除左递归（将花括号提出来，和直接消除左递归结果一样）

<expr> ::= <term> {+ <term>}
         | <term> {- <term>}

<term> ::= <factor> {* <term>}
         | <factor> {/ <term>}

<factor> ::= ( <expr> )
           | Num
*/

#include <stdio.h>
#include <stdlib.h>

enum
{
    Num
};

int expr();
int factor();
int term_tail(int lvalue);

char *src = NULL;
int   token;
int   token_val;

void next();

void match(int tk)
{
    if (token != tk) {
        printf("expected token: %d(%c), got: %d(%c)\n", tk, tk, token, token);
        exit(-1);
    }
    next();
}

int term()
{
    int lvalue = factor();
    return term_tail(lvalue);
}
int expr_tail(int lvalue)
{
    if (token == '+') {
        match('+');
        int value = lvalue + term();
        return expr_tail(value);
    }
    else if (token == '-') {
        match('-');
        int value = lvalue - term();
        return expr_tail(value);
    }
    else {
        return lvalue;
    }
}

int factor()
{
    int value;
    if (token == '(') {
        match('(');
        value = expr();
        match(')');
        return value;
    }
    else {
        value = token_val;
        match(Num);
    }
    return value;
}

int term_tail(int lvalue)
{
    if (token == '*') {
        match('*');
        int value = lvalue * factor();
        return term_tail(value);
    }
    else if (token == '/') {
        match('/');
        int value = lvalue / factor();
        return term_tail(value);
    }
    else {
        return lvalue;
    }
    return 0;
}

int expr()
{
    int lvalue = term();
    return expr_tail(lvalue);
}

void next()
{
    while (*src == ' ' || *src == '\t') {
        src++;
    }
    token = *src++;
    if (token >= '0' && token <= '9') {
        token_val = token - '0';
        token     = Num;
        while (*src >= '0' && *src <= '9') {
            token_val = token_val * 10 + (*src - '0');
            src++;
        }
        return;
    }
}

int main(int argc, char *argv[])
{
    char   *lineptr = NULL;
    size_t  linecap;
    ssize_t linelen;
    while ((linelen = getline(&lineptr, &linecap, stdin))) {
        src = lineptr;
        next();
        printf("%d\n", expr());
    }
    return EXIT_SUCCESS;
}
