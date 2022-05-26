#include "jack_compiler.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void jack_analyzer(char *jack_file_path)
{
    FILE *fp = NULL;
    char *tmp = NULL;
    char *file_name = NULL;
    FILE *op = NULL;
    char **tokens = NULL;

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
    // op to compilation_engine
    fclose(op);
}
