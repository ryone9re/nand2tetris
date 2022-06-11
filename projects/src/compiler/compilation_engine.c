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
    {
        if (strcmp(tokens->word, KEYWORDS[11]) == 0)
        {
            write_push(op, CONST, 1);
            write_arithmetic(op, NEG);
        }
        else if (strcmp(tokens->word, KEYWORDS[12]) == 0)
            write_push(op, CONST, 0);
        else if (strcmp(tokens->word, KEYWORDS[13]) == 0)
            write_push(op, CONST, 0);
        else if (strcmp(tokens->word, KEYWORDS[14]) == 0)
            write_push(op, POINTER, 0);
    }
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
    (void)op;
    if (tokens->token_type == SYMBOL)
    {
    }
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
        write_push(op, CONST, atoi(tokens->word));
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
    {
        write_push(op, CONST, strlen(tokens->word));
        write_call(op, "String", "new", 1);
        for (size_t i = 0; i < strlen(tokens->word); i++)
        {
            write_push(op, CONST, tokens->word[i]);
            write_call(op, "String", "appendChar", 2);
        }
    }
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
    (void)op;
    if (tokens->token_type == IDENTIFIER)
    {
    }
    else
    {
        fprintf(stderr, "%s:%d\tInvaid Identifier: \"%s\"\t\tIdentifier can contain alphabet, number and underscore.\n", file_name, tokens->row, tokens->word);
        exit(1);
    }
    return (tokens->next);
}

Token *write_subroutine_call(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    char *subroutine = NULL;
    char *subroutine_name = NULL;
    int nArgs = 0;

    if (is_identifier(token->word))
    {
        subroutine = token->word;
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->word[0] == SYMBOLS[2])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            nArgs = count_expression(token);
            write_push(op, POINTER, 0);
            token = compile_expression_list(op, token, file_name, class_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
                exit(1);
            }
            write_call(op, class_name, subroutine, nArgs + 1);
        }
        else if (token->word[0] == SYMBOLS[6])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            if (is_identifier(token->word))
            {
                if (index_of(symbol_table, subroutine) >= 0)
                {
                    if (kind_of(symbol_table, subroutine) == Var)
                        write_push(op, LOCAL, index_of(symbol_table, subroutine));
                    else if (kind_of(symbol_table, subroutine) == Argument)
                        write_push(op, ARG, index_of(symbol_table, subroutine));
                    else if (kind_of(symbol_table, subroutine) == Static)
                        write_push(op, STATIC, index_of(symbol_table, subroutine));
                    else if (kind_of(symbol_table, subroutine) == Field)
                        write_push(op, THIS, index_of(symbol_table, subroutine));
                }
                subroutine_name = token->word;
                token = write_identifier(op, token, file_name, symbol_table);
                if (token->word[0] == SYMBOLS[2])
                {
                    token = write_symbol(op, token, file_name, symbol_table);
                    nArgs = count_expression(token);
                    token = compile_expression_list(op, token, file_name, class_name, symbol_table);
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
                if (index_of(symbol_table, subroutine) >= 0)
                {
                    write_call(op, type_of(symbol_table, subroutine), subroutine_name, nArgs + 1);
                }
                else
                    write_call(op, subroutine, subroutine_name, nArgs);
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
    char *class_name = NULL;

    if (strcmp(token->word, KEYWORDS[0]) == 0)
    {
        symbol_table = initialize_symbol_table();
        token = write_keyword(op, token, file_name, symbol_table);
        class_name = token->word;
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
            exit(1);
        }
        if (strcmp(token->word, KEYWORDS[5]) == 0 || strcmp(token->word, KEYWORDS[4]) == 0)
            symbol_table->class = new_symbol();
        while (strcmp(token->word, KEYWORDS[5]) == 0 || strcmp(token->word, KEYWORDS[4]) == 0)
            token = compile_class_var_dec(op, token, file_name, symbol_table);
        while (strcmp(token->word, KEYWORDS[1]) == 0 || strcmp(token->word, KEYWORDS[2]) == 0 || strcmp(token->word, KEYWORDS[3]) == 0)
        {
            symbol_table->subroutine = new_symbol();
            token = compile_subroutine_dec(op, token, file_name, class_name, symbol_table);
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
}

Token *compile_class_var_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *s = NULL;
    Symbol *t = NULL;

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

Token *compile_subroutine_dec(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *c = NULL;
    enum Subroutine_Kind subroutine_kind;
    char *subroutine_name = NULL;

    if (token->token_type == KEYWORD && (strcmp(token->word, KEYWORDS[1]) == 0 || strcmp(token->word, KEYWORDS[2]) == 0 || strcmp(token->word, KEYWORDS[3]) == 0))
    {
        if (strcmp(token->word, KEYWORDS[1]) == 0)
            subroutine_kind = CONSTRUCTOR;
        else if (strcmp(token->word, KEYWORDS[2]) == 0)
            subroutine_kind = FUNCTION;
        else
            subroutine_kind = METHOD;
        if (subroutine_kind == METHOD)
        {
            c = new_symbol();
            c->kind = Argument;
            c->type = strdup(class_name);
            c->name = strdup(KEYWORDS[14]);
            add_symbol(symbol_table->subroutine, c);
        }
        token = write_keyword(op, token, file_name, symbol_table);
        if (strcmp(token->word, KEYWORDS[10]) == 0)
            token = write_keyword(op, token, file_name, symbol_table);
        else
            token = compile_type(op, token, file_name, symbol_table);
        subroutine_name = token->word;
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
        token = compile_subroutine_body(op, token, file_name, class_name, subroutine_name, subroutine_kind, symbol_table);
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Complete Subroutine Call At: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
    return (token);
}

Token *compile_parameter_list(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *s = NULL;

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
    return (token);
}

Token *compile_subroutine_body(FILE *op, Token *tokens, char *file_name, char *class_name, char *subroutine_name, enum Subroutine_Kind subroutine_kind, Symbol_table *symbol_table)
{
    Token *token = tokens;

    if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
    {
        token = write_symbol(op, token, file_name, symbol_table);
        while (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[6]) == 0)
            token = compile_var_dec(op, token, file_name, symbol_table);
        write_function(op, class_name, subroutine_name, var_count(symbol_table, Var));
        if (subroutine_kind == CONSTRUCTOR)
        {
            write_push(op, CONST, var_count(symbol_table, Field));
            write_call(op, "Memory", "alloc", 1);
            write_pop(op, POINTER, 0);
        }
        if (subroutine_kind == METHOD)
        {
            write_push(op, ARG, 0);
            write_pop(op, POINTER, 0);
        }
        token = compile_statements(op, token, file_name, class_name, symbol_table);
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
    return (token);
}

Token *compile_var_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    Symbol *s = NULL;
    Symbol *t = NULL;

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
    return (token);
}

Token *compile_statements(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    while (is_statement(token))
    {
        if (strcmp(token->word, KEYWORDS[15]) == 0)
            token = compile_let(op, token, file_name, class_name, symbol_table);
        else if (strcmp(token->word, KEYWORDS[17]) == 0)
            token = compile_if(op, token, file_name, class_name, symbol_table);
        else if (strcmp(token->word, KEYWORDS[19]) == 0)
            token = compile_while(op, token, file_name, class_name, symbol_table);
        else if (strcmp(token->word, KEYWORDS[16]) == 0)
        {
            token = compile_do(op, token, file_name, class_name, symbol_table);
        }
        else
            token = compile_return(op, token, file_name, class_name, symbol_table);
    }
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

Token *compile_do(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[16]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        token = write_subroutine_call(op, token, file_name, class_name, symbol_table);
        write_pop(op, TEMP, 0);
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
    return (token);
}

Token *compile_let(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    char *variable_name = NULL;
    int is_array = 0;

    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[15]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        variable_name = token->word;
        token = write_identifier(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[4])
        {
            is_array = 1;
            token = write_symbol(op, token, file_name, symbol_table);
            token = compile_expression(op, token, file_name, class_name, symbol_table);
            if (kind_of(symbol_table, variable_name) == Var)
                write_push(op, LOCAL, index_of(symbol_table, variable_name));
            else if (kind_of(symbol_table, variable_name) == Argument)
                write_push(op, ARG, index_of(symbol_table, variable_name));
            else if (kind_of(symbol_table, variable_name) == Static)
                write_push(op, STATIC, index_of(symbol_table, variable_name));
            else if (kind_of(symbol_table, variable_name) == Field)
                write_push(op, THIS, index_of(symbol_table, variable_name));
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[5])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[5], token->word);
                exit(1);
            }
            write_arithmetic(op, ADD);
        }
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[17])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[17], token->word);
            exit(1);
        }
        token = compile_expression(op, token, file_name, class_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
        if (is_array)
        {
            write_pop(op, TEMP, 0);
            write_pop(op, POINTER, 1);
            write_push(op, TEMP, 0);
            write_pop(op, THAT, 0);
        }
        else
        {
            if (kind_of(symbol_table, variable_name) == Var)
                write_pop(op, LOCAL, index_of(symbol_table, variable_name));
            else if (kind_of(symbol_table, variable_name) == Argument)
                write_pop(op, ARG, index_of(symbol_table, variable_name));
            else if (kind_of(symbol_table, variable_name) == Static)
                write_pop(op, STATIC, index_of(symbol_table, variable_name));
            else if (kind_of(symbol_table, variable_name) == Field)
                write_pop(op, THIS, index_of(symbol_table, variable_name));
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[15], token->word, KEYWORDS[15]);
        exit(1);
    }
    return (token);
}

Token *compile_while(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    int l = 0;
    char label[BUFSIZ];

    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[19]) == 0)
    {
        LABEL_NUM++;
        l = LABEL_NUM;
        sprintf(label, "L%d", l);
        write_label(op, label);
        token = write_keyword(op, token, file_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[2])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[2], token->word);
            exit(1);
        }
        token = compile_expression(op, token, file_name, class_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[3], token->word);
            exit(1);
        }
        write_arithmetic(op, NOT);
        LABEL_NUM++;
        l = LABEL_NUM;
        sprintf(label, "L%d", l);
        write_if(op, label);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
            exit(1);
        }
        token = compile_statements(op, token, file_name, class_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
        l--;
        sprintf(label, "L%d", l);
        write_goto(op, label);
        l++;
        sprintf(label, "L%d", l);
        write_label(op, label);
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[19], token->word, KEYWORDS[19]);
        exit(1);
    }
    return (token);
}

Token *compile_return(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[20]) == 0)
    {
        token = write_keyword(op, token, file_name, symbol_table);
        if (is_expression(token))
            token = compile_expression(op, token, file_name, class_name, symbol_table);
        else
            write_push(op, CONST, 0);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[8])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[8], token->word);
            exit(1);
        }
        write_return(op);
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[20], token->word, KEYWORDS[20]);
        exit(1);
    }
    return (token);
}

Token *compile_if(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    int l = 0;
    char label[BUFSIZ];

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
        token = compile_expression(op, token, file_name, class_name, symbol_table);
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
        write_arithmetic(op, NOT);
        LABEL_NUM++;
        l = LABEL_NUM;
        sprintf(label, "L%d", l);
        write_if(op, label);
        token = compile_statements(op, token, file_name, class_name, symbol_table);
        if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
            token = write_symbol(op, token, file_name, symbol_table);
        else
        {
            fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
            exit(1);
        }
        if (token->token_type == KEYWORD && strcmp(token->word, KEYWORDS[18]) == 0)
        {
            LABEL_NUM++;
            sprintf(label, "L%d", LABEL_NUM);
            write_goto(op, label);
            token = write_keyword(op, token, file_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[0])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[0], token->word);
                exit(1);
            }
            sprintf(label, "L%d", l);
            write_label(op, label);
            l = LABEL_NUM;
            token = compile_statements(op, token, file_name, class_name, symbol_table);
            if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[1])
                token = write_symbol(op, token, file_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[1], token->word);
                exit(1);
            }
        }
        sprintf(label, "L%d", l);
        write_label(op, label);
    }
    else
    {
        fprintf(stderr, "%s:%d\tCould Not Find \"%s\" Keyword At: \"%s\"\n\t\tMust begin with \"%s\" declaration.\n", file_name, token->row, KEYWORDS[17], token->word, KEYWORDS[17]);
        exit(1);
    }
    return (token);
}

Token *compile_expression(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    char *operand = NULL;

    token = compile_term(op, token, file_name, class_name, symbol_table);
    while (is_op(token))
    {
        operand = token->word;
        token = write_symbol(op, token, file_name, symbol_table);
        token = compile_term(op, token, file_name, class_name, symbol_table);
        if (operand[0] == SYMBOLS[11])
            write_call(op, "Math", "multiply", 2);
        else if (operand[0] == SYMBOLS[12])
            write_call(op, "Math", "divide", 2);
        else
            write_arithmetic(op, check_binary_op_command(operand));
    }
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

Token *compile_term(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;
    char *operand = NULL;
    char *variable = NULL;

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
                token = compile_expression(op, token, file_name, class_name, symbol_table);
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
            operand = token->word;
            token = write_symbol(op, token, file_name, symbol_table);
            token = compile_term(op, token, file_name, class_name, symbol_table);
            write_arithmetic(op, check_unary_op_command(operand));
        }
        else
        {
            variable = token->word;
            if (token->next->token_type == SYMBOL && token->next->word[0] == SYMBOLS[4])
            {
                token = write_identifier(op, token, file_name, symbol_table);
                token = write_symbol(op, token, file_name, symbol_table);
                if (is_expression(token))
                    token = compile_expression(op, token, file_name, class_name, symbol_table);
                else
                {
                    fprintf(stderr, "%s:%d\tCould Not Find Expression At: \"%s\"\n", file_name, token->row, token->word);
                    exit(1);
                }
                if (kind_of(symbol_table, variable) == Var)
                    write_push(op, LOCAL, index_of(symbol_table, variable));
                else if (kind_of(symbol_table, variable) == Argument)
                    write_push(op, ARG, index_of(symbol_table, variable));
                else if (kind_of(symbol_table, variable) == Static)
                    write_push(op, STATIC, index_of(symbol_table, variable));
                else if (kind_of(symbol_table, variable) == Field)
                    write_push(op, THIS, index_of(symbol_table, variable));
                write_arithmetic(op, ADD);
                if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[5])
                    token = write_symbol(op, token, file_name, symbol_table);
                else
                {
                    fprintf(stderr, "%s:%d\tCould Not Find '%c' Symbol At This Location: \"%s\"\n", file_name, token->row, SYMBOLS[5], token->word);
                    exit(1);
                }
                write_pop(op, POINTER, 1);
                write_push(op, THAT, 0);
            }
            else if (token->next->token_type == SYMBOL && token->next->word[0] == SYMBOLS[6])
                token = write_subroutine_call(op, token, file_name, variable, symbol_table);
            else
            {
                token = write_identifier(op, token, file_name, symbol_table);
                if (kind_of(symbol_table, variable) == Var)
                    write_push(op, LOCAL, index_of(symbol_table, variable));
                else if (kind_of(symbol_table, variable) == Argument)
                    write_push(op, ARG, index_of(symbol_table, variable));
                else if (kind_of(symbol_table, variable) == Static)
                    write_push(op, STATIC, index_of(symbol_table, variable));
                else if (kind_of(symbol_table, variable) == Field)
                    write_push(op, THIS, index_of(symbol_table, variable));
            }
        }
    }
    else
    {
        fprintf(stderr, "%s:%d\tInvalid Term At: \"%s\"\n", file_name, token->row, token->word);
        exit(1);
    }
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

Token *compile_expression_list(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table)
{
    Token *token = tokens;

    if (is_expression(token))
    {
        token = compile_expression(op, token, file_name, class_name, symbol_table);
        while (token->token_type == SYMBOL && token->word[0] == SYMBOLS[7])
        {
            token = write_symbol(op, token, file_name, symbol_table);
            if (is_expression(token))
                token = compile_expression(op, token, file_name, class_name, symbol_table);
            else
            {
                fprintf(stderr, "%s:%d\tCould Not Find Expression At: \"%s\"\n", file_name, token->row, token->word);
                exit(1);
            }
        }
    }
    return (token);
}

int count_expression(Token *token)
{
    int count = 0;
    Token *t = token;

    if (token->token_type == SYMBOL && token->word[0] == SYMBOLS[3])
        return (count);
    count++;
    while (!(t->token_type == SYMBOL && t->word[0] == SYMBOLS[8]))
    {
        if (t->token_type == SYMBOL && t->word[0] == SYMBOLS[7])
            count++;
        t = t->next;
    }
    return (count);
}
