#include "jack_compiler.h"
#include <stdio.h>
#include <stdlib.h>

void compilation_engine(FILE *op, Token *tokens, char *file_name)
{
    compile_class(op, tokens, file_name);
}

Token *write_keyword(FILE *op, Token *tokens, char *file_name)
{
    if (tokens->token_type == KEYWORD)
        fprintf(op, "<keyword> %s </keyword>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Keyword: \"%s\"\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_symbol(FILE *op, Token *tokens, char *file_name)
{
    if (tokens->token_type == SYMBOL)
        fprintf(op, "<symbol> %s </symbol>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Symbol: \"%s\"\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_integer_constant(FILE *op, Token *tokens, char *file_name)
{
    if (tokens->token_type == INT_CONST)
        fprintf(op, "<integerConstant> %s </integerConstant>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Integer: \"%s\"\t\tInteger is decimal number from %d to %d.\n", file_name, tokens->row, tokens->word, MIN_INT, MAX_INT);
        exit(1);
    }
    return (tokens->next);
}

Token *write_string_constant(FILE *op, Token *tokens, char *file_name)
{
    if (tokens->token_type == STRING_CONST)
        fprintf(op, "<stringConstant> %s </stringConstant>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid String: \"%s\"\t\tString is Unicode string not containing double quotes and newline.\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_identifier(FILE *op, Token *tokens, char *file_name)
{
    if (tokens->token_type == IDENTIFIER)
        fprintf(op, "<identifier> %s </identifier>\n", tokens->word);
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Identifier: \"%s\"\t\tIdentifier can contain alphabet, number and underscore.\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

void compile_class(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (strcmp(token->word, KEYWORDS[0]) == 0)
    {
        fprintf(op, "<class>\n");
        token = write_keyword(op, token, file_name);
        token = write_identifier(op, token, file_name);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
            token = write_symbol(op, token, file_name);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
            exit(1);
        }
        if (strcmp(token->word, KEYWORDS[5]) == 0 || strcmp(token->word, KEYWORDS[4]) == 0)
            token = compile_class_var_dec(op, token, file_name);
        if (strcmp(token->word, KEYWORDS[1]) == 0 || strcmp(token->word, KEYWORDS[2]) == 0 || strcmp(token->word, KEYWORDS[3]) == 0)
            token = compile_subroutine(op, token, file_name);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
        fprintf(op, "</class>\n");
    }
    else
    {
        fprintf(stderr, "%s:%d\tIncorrect Token Found: \"%s\"\n\t\tMust be \"%s\".\n", file_name, token->row, token->word, KEYWORDS[0]);
        exit(1);
    }
}

Token *compile_class_var_dec(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (strcmp(token->word, KEYWORDS[5]) == 0)
        token = write_keyword(op, token, file_name);
    else if (strcmp(token->word, KEYWORDS[4]) == 0)
        token = write_keyword(op, token, file_name);
    else
    {
        fprintf(stderr, "%s:%d\tIncomplete Class Declaration: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
    if (token->token_type == KEYWORD)
        token = compile_type(op, token, file_name);
    else if (token->token_type == IDENTIFIER)
        token = write_identifier(op, token, file_name);
    else
    {
        fprintf(stderr, "%s:%d\tIncorrect Token Type: \"%s\" Looking for Keyword or Identifier.\n\t\tToken at this location is of an incorrect type.\n\t\tAt this location only a keyword or identifier is allowed.\n", file_name, token->row, token->word);
        exit(1);
    }
    if (token->token_type == IDENTIFIER)
        token = write_identifier(op, token, file_name);
    else
    {
        fprintf(stderr, "%s:%d\tInvalid Variable: \"%s\"\n\t\tToken Must be only containing alphabet, number and underscore.\n", file_name, token->row, token->word);
        exit(1);
    }
    while (token->word[0] == SYMBOLS[7])
    {
        token = write_symbol(op, token, file_name);
        token = write_identifier(op, token, file_name);
    }
    if (token->word[0] == SYMBOLS[8])
        token = write_symbol(op, token, file_name);
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
        exit(1);
    }
    return (token);
}

Token *compile_type(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (strcmp(token->word, KEYWORDS[7]) == 0)
        token = write_keyword(op, token, file_name);
    else if (strcmp(token->word, KEYWORDS[8]) == 0)
        token = write_keyword(op, token, file_name);
    else if (strcmp(token->word, KEYWORDS[9]) == 0)
        token = write_keyword(op, token, file_name);
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

Token *compile_subroutine(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (token->token_type == KEYWORD && (strcmp(token->word, KEYWORDS[1]) == 0 || strcmp(token->word, KEYWORDS[2]) == 0 || strcmp(token->word, KEYWORDS[3]) == 0))
    {
        token = write_keyword(op, token, file_name);
        if (strcmp(token->word, KEYWORDS[10]) == 0)
            token = write_keyword(op, token, file_name);
        else
            token = compile_type(op, token, file_name);
        token = write_identifier(op, token, file_name);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
            token = write_keyword(op, token, file_name);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], token->word);
            exit(1);
        }
        token = compile_parameter_list(op, token, file_name);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
            token = write_keyword(op, token, file_name);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
            exit(1);
        }
        token = compile_subroutine_body(op, token, file_name);
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Complete Subroutine Call At: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
    return (token);
}

Token *compile_parameter_list(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (is_type(token))
    {
        token = compile_type(op, token, file_name);
        token = write_identifier(op, token, file_name);
        while (token->token_type == SYMBOL && token->word[0] == SYMBOLS[7])
        {
            token = compile_type(op, token, file_name);
            token = write_identifier(op, token, file_name);
        }
    }
    return (token);
}

Token *compile_subroutine_body(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
    {
        token = write_symbol(op, token, file_name);
        if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[6]) == 0)
            token = compile_var_dec(op, token, file_name);
        token = compile_statements(op, token, file_name);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
        exit(1);
    }
    return (token);
}

Token *compile_var_dec(FILE *op, Token *tokens, char *file_name)
{
    Token *token = tokens;

    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[6]) == 0)
    {
        token = write_keyword(op, token, file_name);
        token = compile_type(op, token, file_name);
        token = write_identifier(op, token, file_name);
        while (token->token_type == SYMBOL && token->word[0] == SYMBOLS[7])
        {
            token = write_symbol(op, token, file_name);
            token = compile_type(op, token, file_name);
            token = write_identifier(op, token, file_name);
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%s' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
        //
    }
    else
    {
        fprintf(stderr, "%s:%d\tIncorrect Token Found: \"%s\"\n\t\tMust be \"%s\".\n", file_name, token->row, token->word, KEYWORDS[6]);
        exit(1);
    }
}

Token *compile_statements(FILE *op, Token *tokens, char *file_name);

Token *compile_do(FILE *op, Token *tokens, char *file_name);

Token *compile_let(FILE *op, Token *tokens, char *file_name);

Token *compile_while(FILE *op, Token *tokens, char *file_name);

Token *compile_return(FILE *op, Token *tokens, char *file_name);

Token *compile_if(FILE *op, Token *tokens, char *file_name);

Token *compile_expression(FILE *op, Token *tokens, char *file_name);

Token *compile_term(FILE *op, Token *tokens, char *file_name);

Token *compile_expression_list(FILE *op, Token *tokens, char *file_name);
