#include "Postfix.h"


Token* pop(Token** stack, int* top)
{
    if (top == -1)
        return -1;
    else
        return stack[(*top)--];
}

int priority(Token* x)
{
    if (x->type == TOKEN_LEFT_PAREN)
        return 0;
    if (x->type == TOKEN_GREATER || x->type == TOKEN_GREATER_EQUAL || x->type == TOKEN_LESS_EQUAL || x->type == TOKEN_LESS)
        return 1;
    if (x->type == TOKEN_PLUS || x->type == TOKEN_MINUS)
        return 2;
    if (x->type == TOKEN_STAR || x->type == TOKEN_SLASH)
        return 3;
    return 0;
}

int convertToPost(Parser* par, ParseTree* current, TokenType EO_Expr)
{
    int openParenthesis = 0;
    Token* token;
    Token* stack[100];
    int top = -1;
    stack[++top] = par->current;
    token = pop(stack, &top);

    while (par->current->type != EO_Expr && par->current->type != TOKEN_EOF)
    {
        if (par->current->type == TOKEN_NUMBER || par->current->type == TOKEN_IDENTIFIER)
        {
            ParseTree* child = newTree(getType(par, par->current), par->current);
            current->addChild(current, child);
            printf("%s ", par->current->lexeme);
        }
        else if (par->current->type == TOKEN_LEFT_PAREN) {
            openParenthesis++;
            stack[++top] = par->current;
        }
        else if (par->current->type == TOKEN_RIGHT_PAREN)
        {
            if (openParenthesis == 0) {
                error(par, par->current, "Invalid Syntax");
                synchronize(par);
            }
            openParenthesis--;
            token = pop(stack, &top);
            while (top != -1 && token->type != TOKEN_LEFT_PAREN) {
                ParseTree* child = newTree(getType(par, token), token);
                current->addChild(current, child);
                printf("%s ", token->lexeme);
                token = pop(stack, &top);
            }
        }
        else
        {
            while (top != -1 && priority(stack[top]) >= priority(par->current))
            {
                token = pop(stack, &top);
                ParseTree* child = newTree(getType(par, token), token);
                current->addChild(current, child);
                printf("%s ", token->lexeme);
            }
            stack[++top] = par->current;
        }

        parserAdvance(par);
    }

    while (top != -1)
    {
        token = pop(stack, &top);
        ParseTree* child = newTree(getType(par, token), token);
        current->addChild(current, child);
        printf("%s ", token->lexeme);
    }
    if (openParenthesis)
    {
        error(par, par->current, "Invalid Syntax");
        synchronize(par);
    }
    return 0;
}

ParseTreeType getType(Parser* par, Token* token)
{
    switch (token->type)
    {
    case TOKEN_IDENTIFIER:
        return IDENTIFIER_PARSE;

    case TOKEN_NUMBER:
        return ATOMIC_PARSE;

    case TOKEN_PLUS:
        return PARSE_PLUS;
    case TOKEN_MINUS:
        return PARSE_MINUS;
    case TOKEN_STAR:
        return PARSE_STAR;
    case TOKEN_SLASH:
        return PARSE_SLASH;

    case TOKEN_GREATER:
        return PARSE_GREATER;
    case TOKEN_GREATER_EQUAL:
        return PARSE_GREATER_EQUAL;
    case TOKEN_LESS:
        return PARSE_LESS;
    case TOKEN_LESS_EQUAL:
        return PARSE_LESS_EQUAL;

    default:
        error(par, token, "Invalid Syntax");
        synchronize(par);
        break;
    }
}