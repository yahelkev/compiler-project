#include "Postfix.h"



Token* pop(Token** stack, int* top) {
    if (top == EMPTY_STACK)
        return EMPTY_STACK;
    else
        return stack[(*top)--];
}

Token* peekPost(Token** stack, int* top) {
    if(top == EMPTY_STACK)
        return EMPTY_STACK;
    else
        return stack[(*top)];
}

int priority(Token* x) {
    if (x->type == TOKEN_LEFT_PAREN)
        return 0;
    if (x->type == TOKEN_GREATER || x->type == TOKEN_GREATER_EQUAL || x->type == TOKEN_LESS_EQUAL || x->type == TOKEN_LESS)
        return CON_PRIORITY;
    if (x->type == TOKEN_PLUS || x->type == TOKEN_MINUS)
        return PLUS_MINUS_PRIORITY;
    if (x->type == TOKEN_STAR || x->type == TOKEN_SLASH)
        return MUL_DIV_PRIORITY;
    return 0;
}

int convertToPost(Parser* par, ParseTree* current, TokenType EO_Expr) {
    int openParenthesis = 0, numNiden = 0, operators = 0, top = -1;;
    Token* token = par->current, *stack[MAX_STACK_SIZE];
    ParseTree* child = NULL, *lastChild = NULL, *twoLastChild = NULL, *threeLastChild = NULL;
    

    while (par->current->type != EO_Expr && par->current->type != TOKEN_EOF)
    {
        if (par->current->type == TOKEN_INT || par->current->type == TOKEN_FLOAT || par->current->type == TOKEN_IDENTIFIER || par->current->type == TOKEN_STRING)
        {
            numNiden++;
            if ((numNiden - operators) >= 2) {
                error(par, par->current, "Too many operators");
                synchronize(par);
                return 0;
            }
            child = newTree(getType(par, par->current), par->current);
            lastChild = current->getChild(current, current->amountOfChilds - 1);
            twoLastChild = current->getChild(current, current->amountOfChilds - 2);
            // Check here if last child is a constant and then fold it with top stack
            if (lastChild && twoLastChild && child->type == ATOMIC_PARSE && twoLastChild->type == ATOMIC_PARSE && priority(peekPost(stack, &top)) >= priority(lastChild ->token)) {
                // Fold constants
                // twoLastChild 3
                // lastChild +
                // child 5
                float firstConst = atof(twoLastChild->token->lexeme);
                float secondConst = atof(child->token->lexeme);
                float out = 0;
                switch (lastChild->type) {
                case PARSE_PLUS:
                    out = firstConst + secondConst;
                    break;
                case PARSE_MINUS:
                    out = firstConst - secondConst;
                    break;
                case PARSE_STAR:
                    out = firstConst * secondConst;
                    break;
                case PARSE_SLASH:
                    out = firstConst / secondConst;
                    break;
                }
                Token* foldToken = (Token*)malloc(sizeof(Token));
                foldToken->column = twoLastChild->token->column;
                foldToken->line = twoLastChild->token->line;
                foldToken->type = out == (int)out ? TOKEN_INT : TOKEN_FLOAT;
                foldToken->lexeme = (char*)malloc((int)((ceil(log10((int)out)) + 1) * sizeof(char)) + 3);
                out == (int)out ? sprintf(foldToken->lexeme, "%d", (int)out) : sprintf(foldToken->lexeme, "%.3f", out);
                foldToken->length = strlen(foldToken->lexeme);

                child = newTree(ATOMIC_PARSE, foldToken);
                current->delChild(current);
                current->delChild(current);
                printf("Fold\n");
            }
            current->addChild(current, child);
        }
        else if (par->current->type == TOKEN_LEFT_PAREN) {
            if (numNiden > operators) {
                error(par, par->current, "Unexpected '('");
                synchronize(par);
                return 0;
            }
            openParenthesis++;
            stack[++top] = par->current;
        }
        else if (par->current->type == TOKEN_RIGHT_PAREN)
        {
            if (openParenthesis == 0 || numNiden == operators) {
                error(par, par->current, "Unexpected ')'");
                synchronize(par);
                return 0;
            }
            openParenthesis--;
            token = pop(stack, &top);
            while (top != -1 && token->type != TOKEN_LEFT_PAREN) {
                child = newTree(getType(par, token), token);
                current->addChild(current, child);
                token = pop(stack, &top);
            }
        }
        else
        {
            operators++;
            if (operators > numNiden) {
                error(par, par->current, "Invalid identifier");
                synchronize(par);
                return 0;
            }
            while (top != -1 && priority(peekPost(stack, &top)) >= priority(par->current))
            {
                token = pop(stack, &top);
                child = newTree(getType(par, token), token);
                current->addChild(current, child);
            }
            stack[++top] = par->current;
        }
        parserAdvance(par);
    }

    if (openParenthesis)
    {
        error(par, par->pre, "Unclosed parenthesis");
        synchronize(par);
        return 0;
    }
    while (top != -1)
    {
        token = pop(stack, &top);
        ParseTree* child = newTree(getType(par, token), token);
        current->addChild(current, child);
    }
    return 1;
}

ParseTreeType getType(Parser* par, Token* token)
{
    switch (token->type)
    {
    case TOKEN_IDENTIFIER:
        return IDENTIFIER_PARSE;

    case TOKEN_INT:
    case TOKEN_FLOAT:
        return ATOMIC_PARSE;
    case TOKEN_STRING:
        return PARSE_STRING;

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
        error(par, token, "Unexpected operator");
        synchronize(par);
        return PARSE_ERROR;
    }
}
