#include "jack_compiler.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// test
void write_T(FILE *op, Token *tokens);

void jack_analyzer(char *jack_file_path)
{
    FILE *fp = NULL;
    char *tmp = NULL;
    char *file_name = NULL;
    FILE *op = NULL;
    Token *tokens = NULL;

    fp = fopen(jack_file_path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENONET);
    }
    tmp = remove_extension(jack_file_path);
    if (tmp == NULL)
    {
        fprintf(stderr, "%s:%s\n", jack_file_path, strerror(ENOMEM));
        return;
    }
    file_name = add_extension(tmp, ".xml");
    free(tmp);
    if (file_name == NULL)
    {
        fprintf(stderr, "%s:%s\n", jack_file_path, strerror(ENOMEM));
        return;
    }
    op = fopen(file_name, "w");
    free(file_name);
    // fp to tokenize
    tokens = jack_tokenizer(fp);
    fclose(fp);
    if (tokens == NULL)
    {
        fprintf(stderr, "%s:%s\n", jack_file_path, strerror(ENOMEM));
        return;
    }
    write_T(op, tokens);
    // op to compilation_engine
    fclose(op);
}

// test code
// enum Token_type
// {
//     NOTOKEN,
//     KEYWORD,
//     SYMBOL,
//     IDENTIFIER,
//     INT_CONST,
//     STRING_CONST
// };
void write_T(FILE *op, Token *tokens)
{
    Token *t = tokens;

    fprintf(op, "<tokens>\n");
    while (t != NULL)
    {
        if (t->token_type == KEYWORD)
            fprintf(op, "<keyword> %s </keyword>\n", t->word);
        else if (t->token_type == SYMBOL)
            fprintf(op, "<symbol> %s </symbol>\n", t->word);
        else if (t->token_type == IDENTIFIER)
            fprintf(op, "<identifier> %s </identifier>\n", t->word);
        else if (t->token_type == INT_CONST)
            fprintf(op, "<integerConstant> %s </integerConstant>\n", t->word);
        else
            fprintf(op, "<stringConstant> %s </stringConstant>\n", t->word);
        t = t->next;
    }
    fprintf(op, "</tokens>\n");
}
