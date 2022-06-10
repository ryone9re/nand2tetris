#include "jack_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compilation_engine(FILE *op, Token *tokens, char *file_name)
{
    compile_class(op, tokens, file_name);
}

Token *write_keyword(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    (void)symbol_table;
    if (tokens->token_type == KEYWORD)
        fprintf(op, "<keyword> %s </keyword>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Keyword: \"%s\"\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_symbol(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    (void)symbol_table;
    if (tokens->token_type == SYMBOL)
        fprintf(op, "<symbol> %s </symbol>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Symbol: \"%s\"\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_integer_constant(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    (void)symbol_table;
    if (tokens->token_type == INT_CONST)
        fprintf(op, "<integerConstant> %s </integerConstant>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Integer: \"%s\"\t\tInteger is decimal number from %d to %d.\n", file_name, tokens->row, tokens->word, MIN_INT, MAX_INT);
        exit(1);
    }
    return (tokens->next);
}

Token *write_string_constant(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    (void)symbol_table;
    for (int i = 1; i <= (int)strlen(tokens->word); i++)
        tokens->word[i - 1] = tokens->word[i];
    for (int i = (int)strlen(tokens->word) - 1; 0 <= i; i--)
    {
        if (tokens->word[i] == '"')
        {
            tokens->word[i] = '\0';
            break;
        }
    }
    if (tokens->token_type == STRING_CONST)
        fprintf(op, "<stringConstant> %s </stringConstant>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid String: \"%s\"\t\tString is Unicode string not containing double quotes and newline.\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_identifier(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    (void)symbol_table;
    if (tokens->token_type == IDENTIFIER)
        fprintf(op, "<identifier> %s </identifier>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Identifier: \"%s\"\t\tIdentifier can contain alphabet, number and underscore.\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_subroutine_call(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    if (is_identifier(token->word))
    {
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->word[0] == SYMBOLS[2])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            token = compile_expression_list(op, token, file_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
                exit(1);
            }
        }
        else if (token->word[0] == SYMBOLS[6])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            if (is_identifier(token->word))
            {
                token = write_identifier(op, token, file_name, symbol_table);
                if (token->word[0] == SYMBOLS[2])
                {
                    token = write_symbol(op, token, file_name, symbol_table);
                    token = compile_expression_list(op, token, file_name, symbol_table);
                    if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
                        token = write_symbol(op, token, file_name, symbol_table);
                    else
                    {
                        fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
                        exit(1);
                    }
                }
                else
                {
                    fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], token->word);
                    exit(1);
                }
            }
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Complete Subroutine Call At: \"%s\"\t\tIncomplete Program.\n", file_name, tokens->row, tokens->word);
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' or '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], SYMBOLS[6], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Complete Subroutine Call At: \"%s\"\t\tIncomplete Program.\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (token);
}

void compile_class(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;
    Symbol_table *symbol_table = NULL;

    fprintf(op, "<class>\n");
    if (strcmp(token->word, KEYWORDS[0]) == 0)
    {
        symbol_table = initialize_symbol_table();
        token = write_keyword(op, token, file_name, symbol_table);
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
            exit(1);
        }
        while (strcmp(token->word, KEYWORDS[5]) == 0 || strcmp(token->word, KEYWORDS[4]) == 0)
        {
            symbol_table->class = new_symbol();
            token = compile_class_var_dec(op, token, file_name, symbol_table);
        }
        while (strcmp(token->word, KEYWORDS[1]) == 0 || strcmp(token->word, KEYWORDS[2]) == 0 || strcmp(token->word, KEYWORDS[3]) == 0)
        {
            symbol_table->subroutine = new_symbol();
            token = compile_subroutine_dec(op, token, file_name, symbol_table);
            free_symbol(symbol_table->subroutine);
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
        free_symbol(symbol_table->class);
        free_symbol_table(symbol_table);
    }
    else
    {
        fprintf(stderr, "%s:%d\tIncorrect Keyword Found: \"%s\"\n\t\tMust be \"%s\".\n", file_name, token->row, token->word, KEYWORDS[0]);
        exit(1);
    }
    fprintf(op, "</class>\n");
}

Token *compile_class_var_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *s = NULL;
    Symbol *t = NULL;

    fprintf(op, "<classVarDec>\n");
    if (token->token_type == KEYWORD && (strcmp(token->word, KEYWORDS[5]) == 0 || strcmp(token->word, KEYWORDS[4]) == 0))
    {
        s = new_symbol();
        if (strcmp(token->word, KEYWORDS[5]) == 0)
        {
            s->kind = Static;
            token = write_keyword(op, token, file_name, symbol_table);
        }
        else
        {
            s->kind = Field;
            token = write_keyword(op, token, file_name, symbol_table);
        }
        if (token->token_type == KEYWORD)
        {
            s->type = strdup(token->word);
            token = compile_type(op, token, file_name, symbol_table);
        }
        else if (token->token_type == IDENTIFIER)
        {
            s->type = strdup(token->word);
            token = write_identifier(op, token, file_name, symbol_table);
        }
        else
        {
            fprintf(stderr, "%s:%d\tIncorrect Token Type: \"%s\" Looking for Keyword or Identifier.\n\t\tToken at this location is of an incorrect type.\n\t\tAt this location only a keyword or identifier is allowed.\n", file_name, token->row, token->word);
            exit(1);
        }
        if (token->token_type == IDENTIFIER)
        {
            s->name = strdup(token->word);
            token = write_identifier(op, token, file_name, symbol_table);
        }
        else
        {
            fprintf(stderr, "%s:%d\tInvalid Variable: \"%s\"\n\t\tToken Must be only containing alphabet, number and underscore.\n", file_name, token->row, token->word);
            exit(1);
        }
        s = add_symbol(symbol_table->class, s);
        while (token->word[0] == SYMBOLS[7])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            t = new_symbol();
            t->kind = s->kind;
            t->type = strdup(s->type);
            t->name = strdup(token->word);
            token = write_identifier(op, token, file_name, symbol_table);
            add_symbol(symbol_table->class, t);
        }
        if (token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tIncomplete Class Declaration: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
    fprintf(op, "</classVarDec>\n");
    return (token);
}

Token *compile_type(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    if (strcmp(token->word, KEYWORDS[7]) == 0)
        token = write_keyword(op, token, file_name, symbol_table);
    else if (strcmp(token->word, KEYWORDS[8]) == 0)
        token = write_keyword(op, token, file_name, symbol_table);
    else if (strcmp(token->word, KEYWORDS[9]) == 0)
        token = write_keyword(op, token, file_name, symbol_table);
    else if (token->token_type == IDENTIFIER && is_identifier(token->word))
        token = write_identifier(op, token, file_name, symbol_table);
    else
    {
        fprintf(stderr, "%s:%d\tIncorrect Token Type: \"%s\" Looking for Keyword or Identifier.\n\t\tToken at this location is of an incorrect type.\n\t\tAt this location only a keyword or identifier is allowed.\n", file_name, token->row, token->word);
        exit(1);
    }
    return (token);
}

int is_type(Token *token)
{
    if (token->token_type == KEYWORD)
    {
        if (strcmp(token->word, KEYWORDS[7]) == 0)
            return (1);
        if (strcmp(token->word, KEYWORDS[8]) == 0)
            return (1);
        if (strcmp(token->word, KEYWORDS[9]) == 0)
            return (1);
    }
    if (token->token_type == IDENTIFIER)
    {
        if (is_identifier(token->word))
            return (1);
    }
    return (0);
}

Token *compile_subroutine_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<subroutineDec>\n");
    if (token->token_type == KEYWORD && (strcmp(token->word, KEYWORDS[1]) == 0 || strcmp(token->word, KEYWORDS[2]) == 0 || strcmp(token->word, KEYWORDS[3]) == 0))
    {
        token = write_keyword(op, token, file_name, symbol_table);
        if (strcmp(token->word, KEYWORDS[10]) == 0)
            token = write_keyword(op, token, file_name, symbol_table);
        else
            token = compile_type(op, token, file_name, symbol_table);
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], token->word);
            exit(1);
        }
        token = compile_parameter_list(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
            exit(1);
        }
        token = compile_subroutine_body(op, token, file_name, symbol_table);
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Complete Subroutine Call At: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
    fprintf(op, "</subroutineDec>\n");
    return (token);
}

Token *compile_parameter_list(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *s = NULL;

    fprintf(op, "<parameterList>\n");
    if (is_type(token))
    {
        s = new_symbol();
        s->kind = Argument;
        s->type = strdup(token->word);
        token = compile_type(op, token, file_name, symbol_table);
        s->name = strdup(token->word);
        token = write_identifier(op, token, file_name, symbol_table);
        add_symbol(symbol_table->subroutine, s);
        while (token->token_type == SYMBOL && token->word[0] == SYMBOLS[7])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            s = new_symbol();
            s->kind = Argument;
            s->type = strdup(token->word);
            token = compile_type(op, token, file_name, symbol_table);
            s->name = strdup(token->word);
            token = write_identifier(op, token, file_name, symbol_table);
            add_symbol(symbol_table->subroutine, s);
        }
    }
    fprintf(op, "</parameterList>\n");
    return (token);
}

Token *compile_subroutine_body(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<subroutineBody>\n");
    if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
    {
        token = write_symbol(op, token, file_name, symbol_table);
        while (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[6]) == 0)
            token = compile_var_dec(op, token, file_name, symbol_table);
        token = compile_statements(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
        exit(1);
    }
    fprintf(op, "</subroutineBody>\n");
    return (token);
}

Token *compile_var_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *s = NULL;
    Symbol *t = NULL;

    fprintf(op, "<varDec>\n");
    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[6]) == 0)
    {
        s = new_symbol();
        s->kind = Var;
        token = write_keyword(op, token, file_name, symbol_table);
        s->type = strdup(token->word);
        token = compile_type(op, token, file_name, symbol_table);
        s->name = strdup(token->word);
        token = write_identifier(op, token, file_name, symbol_table);
        s = add_symbol(symbol_table->subroutine, s);
        while (token->token_type == SYMBOL && token->word[0] == SYMBOLS[7])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            t = new_symbol();
            t->kind = s->kind;
            t->type = strdup(s->type);
            t->name = strdup(token->word);
            token = write_identifier(op, token, file_name, symbol_table);
            add_symbol(symbol_table->subroutine, t);
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tIncorrect Keyword Found: \"%s\"\n\t\tMust be \"%s\".\n", file_name, token->row, token->word, KEYWORDS[6]);
        exit(1);
    }
    fprintf(op, "</varDec>\n");
    return (token);
}

Token *compile_statements(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<statements>\n");
    while (is_statement(token))
    {
        if (strcmp(token->word, KEYWORDS[15]) == 0)
            token = compile_let(op, token, file_name, symbol_table);
        else if (strcmp(token->word, KEYWORDS[17]) == 0)
            token = compile_if(op, token, file_name, symbol_table);
        else if (strcmp(token->word, KEYWORDS[19]) == 0)
            token = compile_while(op, token, file_name, symbol_table);
        else if (strcmp(token->word, KEYWORDS[16]) == 0)
            token = compile_do(op, token, file_name, symbol_table);
        else
            token = compile_return(op, token, file_name, symbol_table);
    }
    fprintf(op, "</statements>\n");
    return (token);
}

int is_statement(Token *token)
{
    if (strcmp(token->word, KEYWORDS[15]) == 0)
        return (1);
    else if (strcmp(token->word, KEYWORDS[17]) == 0)
        return (1);
    else if (strcmp(token->word, KEYWORDS[19]) == 0)
        return (1);
    else if (strcmp(token->word, KEYWORDS[16]) == 0)
        return (1);
    else if (strcmp(token->word, KEYWORDS[20]) == 0)
        return (1);
    else
        return (0);
}

Token *compile_do(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<doStatement>\n");
    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[16]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        token = write_subroutine_call(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tSubroutine calls must begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[16], token->word, KEYWORDS[16]);
        exit(1);
    }
    fprintf(op, "</doStatement>\n");
    return (token);
}

Token *compile_let(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<letStatement>\n");
    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[15]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[4])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            token = compile_expression(op, token, file_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[5])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[5], token->word);
                exit(1);
            }
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[17])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[17], token->word);
            exit(1);
        }
        token = compile_expression(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[15], token->word, KEYWORDS[15]);
        exit(1);
    }
    fprintf(op, "</letStatement>\n");
    return (token);
}

Token *compile_while(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<whileStatement>\n");
    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[19]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], token->word);
            exit(1);
        }
        token = compile_expression(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
            exit(1);
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
            exit(1);
        }
        token = compile_statements(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[19], token->word, KEYWORDS[19]);
        exit(1);
    }
    fprintf(op, "</whileStatement>\n");
    return (token);
}

Token *compile_return(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<returnStatement>\n");
    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[20]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        if (is_expression(token))
            token = compile_expression(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[20], token->word, KEYWORDS[20]);
        exit(1);
    }
    fprintf(op, "</returnStatement>\n");
    return (token);
}

Token *compile_if(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<ifStatement>\n");
    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[17]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], token->word);
            exit(1);
        }
        token = compile_expression(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
            exit(1);
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
            exit(1);
        }
        token = compile_statements(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
        if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[18]) == 0)
        {
            token = write_keyword(op, token, file_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
                exit(1);
            }
            token = compile_statements(op, token, file_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
                exit(1);
            }
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[17], token->word, KEYWORDS[17]);
        exit(1);
    }
    fprintf(op, "</ifStatement>\n");
    return (token);
}

Token *compile_expression(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<expression>\n");
    token = compile_term(op, token, file_name, symbol_table);
    while (is_op(token))
    {
        token = write_symbol(op, token, file_name, symbol_table);
        token = compile_term(op, token, file_name, symbol_table);
    }
    fprintf(op, "</expression>\n");
    return (token);
}

int is_expression(Token *token)
{
    if (is_term(token))
        return (1);
    return (0);
}

int is_op(Token *token)
{
    if (token->token_type == SYMBOL)
    {
        if (token->word[0] == SYMBOLS[9])
            return (1);
        else if (token->word[0] == SYMBOLS[10])
            return (1);
        else if (token->word[0] == SYMBOLS[11])
            return (1);
        else if (token->word[0] == SYMBOLS[12])
            return (1);
        else if (token->word[0] == SYMBOLS[13])
            return (1);
        else if (token->word[0] == SYMBOLS[14])
            return (1);
        else if (token->word[0] == SYMBOLS[15])
            return (1);
        else if (token->word[0] == SYMBOLS[16])
            return (1);
        else if (token->word[0] == SYMBOLS[17])
            return (1);
        else
            return (0);
    }
    return (0);
}

int is_unary_op(Token *token)
{
    if (token->token_type == SYMBOL)
    {
        if (token->word[0] == SYMBOLS[10])
            return (1);
        else if (token->word[0] == SYMBOLS[18])
            return (1);
        else
            return (0);
    }
    return (0);
}

Token *compile_term(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<term>\n");
    if (is_term(token))
    {
        if (is_integer_constant(token->word))
            token = write_integer_constant(op, token, file_name, symbol_table);
        else if (is_string_constant(token->word))
            token = write_string_constant(op, token, file_name, symbol_table);
        else if (is_keyword_constant(token->word))
            token = write_keyword(op, token, file_name, symbol_table);
        else if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            if (is_expression(token))
                token = compile_expression(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find Expression At: \"%s\"\n", file_name, token->row, token->word);
                exit(1);
            }
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
                exit(1);
            }
        }
        else if (is_unary_op(token))
        {
            token = write_symbol(op, token, file_name, symbol_table);
            token = compile_term(op, token, file_name, symbol_table);
        }
        else
        {
            token = write_identifier(op, token, file_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[4])
            {
                token = write_symbol(op, token, file_name, symbol_table);
                if (is_expression(token))
                    token = compile_expression(op, token, file_name, symbol_table);
                else
                {
                    fprintf(stderr, "%s:%d\tCould Not Find Expression At: \"%s\"\n", file_name, token->row, token->word);
                    exit(1);
                }
                if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[5])
                    token = write_symbol(op, token, file_name, symbol_table);
                else
                {
                    fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[5], token->word);
                    exit(1);
                }
            }
            else if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[6])
            {
                token = write_symbol(op, token, file_name, symbol_table);
                token = write_subroutine_call(op, token, file_name, symbol_table);
            }
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tInvalid Term At: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
    fprintf(op, "</term>\n");
    return (token);
}

int is_term(Token *token)
{
    if (is_integer_constant(token->word))
        return (1);
    if (is_string_constant(token->word))
        return (1);
    if (is_keyword_constant(token->word))
        return (1);
    if (is_identifier(token->word))
        return (1);
    if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
        return (1);
    if (is_unary_op(token))
        return (1);
    return (0);
}

Token *compile_expression_list(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    fprintf(op, "<expressionList>\n");
    if (is_expression(token))
    {
        token = compile_expression(op, token, file_name, symbol_table);
        while (token->token_type == SYMBOL && token->word[0] == SYMBOLS[7])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            if (is_expression(token))
                token = compile_expression(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find Expression At: \"%s\"\n", file_name, token->row, token->word);
                exit(1);
            }
        }
    }
    fprintf(op, "</expressionList>\n");
    return (token);
}
