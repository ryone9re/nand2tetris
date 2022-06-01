#include "jack_compiler.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Token *jack_tokenizer(FILE *fp)
{
    char c, d = 0;
    int word_len = 0;
    char *word = NULL;
    char *tmp_word = NULL;
    Token *tokens = NULL;
    int in_comment = 0;

    while ((c = getc(fp)) != EOF)
    {
        if (c == '/')
        {
            if ((d = getc(fp)) == '/' || d == '*')
            {
                in_comment = 1;
                continue;
            }
            else
                ungetc(d, fp);
        }
        if (in_comment && c == '\n')
        {
            in_comment = 0;
            continue;
        }
        if (in_comment && c == '*')
        {
            if ((d = getc(fp)) == '/')
            {
                in_comment = 0;
                continue;
            }
            else
                ungetc(d, fp);
        }
        if (!in_comment)
        {
            if (word == NULL)
                word_len = 0;
            else
                word_len = strlen(word);
            tmp_word = (char *)realloc(word, sizeof(char) * (word_len + 1) + sizeof(char));
            if (tmp_word == NULL)
            {
                free(word);
                return (NULL);
            }
            word = tmp_word;
            word[word_len] = c;
            word[word_len + 1] = '\0';
            // TODO string constant内にスペースがあったときの処理
            if (isspace(c) || is_symbol(c))
            {
                if (is_spaces(word))
                {
                    free(word);
                    word = NULL;
                    continue;
                }
                else if (is_symbol(c) && strlen(word) == 1)
                {
                    tokens = add_token(tokens, word, SYMBOL);
                    free(word);
                    word = NULL;
                    continue;
                }
                else
                {
                    word[strlen(word) - 1] = '\0';
                    if (is_symbol(c))
                        ungetc(c, fp);
                    if (is_keyword(word))
                    {
                        tokens = add_token(tokens, word, KEYWORD);
                        free(word);
                        word = NULL;
                        continue;
                    }
                    else
                    {
                        if (is_integer_constant(word))
                            tokens = add_token(tokens, word, INT_CONST);
                        else if (is_string_constant(word))
                            tokens = add_token(tokens, word, STRING_CONST);
                        else if (is_identifier(word))
                            tokens = add_token(tokens, word, IDENTIFIER);
                        else
                        {
                            fprintf(stderr, "%s is invalid\n", word);
                            exit(1);
                        }
                        free(word);
                        word = NULL;
                        continue;
                    }
                }
            }
        }
    }
    if (word != NULL)
        free(word);
    return (tokens);
}

int is_spaces(char *str)
{
    while (*str != '\0')
    {
        if (!isspace(*str))
            return (0);
        str++;
    }
    return (1);
}

int is_integer_constant(char *str)
{
    int i = 0;

    if (strlen(str) == 1 && *str == '0')
        return (1);
    i = atoi(str);
    if (i)
        return (1);
    return (0);
}

int is_string_constant(char *str)
{
    if (str[0] != '"' || str[strlen(str) - 1] != '"')
    {
        return (0);
    }
    while (*str != '\0')
    {
        if (*str == '\r' || *str == '\n')
            return (0);
        str++;
    }
    return (1);
}

int is_identifier(char *str)
{
    if (isdigit(str[0]))
        return (0);
    while (*str != '\0')
    {
        if (!isalnum(*str) && *str != '_')
            return (0);
        str++;
    }
    return (1);
}

int is_symbol(char c)
{
    for (int i = 0; i < SYMBOL_COUNT; i++)
    {
        if (c == SYMBOLS[i])
            return (1);
    }
    return (0);
}

int is_keyword(char *str)
{
    for (int i = 0; i < KEYWORD_COUNT; i++)
    {
        if (strcmp(str, KEYWORDS[i]) == 0)
            return (1);
    }
    return (0);
}

Token *new_token(Token *tokens)
{
    Token *new_token = NULL;
    Token *last_token = tokens;

    if (tokens == NULL)
    {
        tokens = (Token *)malloc(sizeof(Token));
        tokens->word = NULL;
        tokens->token_type = NOTOKEN;
        tokens->next = NULL;
    }
    else
    {
        new_token = (Token *)malloc(sizeof(Token));
        new_token->word = NULL;
        new_token->token_type = NOTOKEN;
        new_token->next = NULL;
        while (last_token->next != NULL)
            last_token = last_token->next;
        last_token->next = new_token;
    }
    return (tokens);
}

Token *add_token(Token *tokens, char *word, enum Token_type token_type)
{
    Token *token = NULL;
    Token *curr = NULL;

    token = new_token(tokens);
    curr = token;
    while (curr->next != NULL)
        curr = curr->next;
    curr->word = strdup(word);
    curr->token_type = token_type;
    curr->next = NULL;
    return (token);
}
