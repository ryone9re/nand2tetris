#include "jack_compiler.h"
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const char *KEYWORDS[KEYWORD_COUNT] = {
    "class",       /* 0 */
    "constructor", /* 1 */
    "function",    /* 2 */
    "method",      /* 3 */
    "field",       /* 4 */
    "static",      /* 5 */
    "var",         /* 6 */
    "int",         /* 7 */
    "char",        /* 8 */
    "boolean",     /* 9 */
    "void",        /* 10 */
    "true",        /* 11 */
    "false",       /* 12 */
    "null",        /* 13 */
    "this",        /* 14 */
    "let",         /* 15 */
    "do",          /* 16 */
    "if",          /* 17 */
    "else",        /* 18 */
    "while",       /* 19 */
    "return",      /* 20 */
};

const char SYMBOLS[SYMBOL_COUNT] = {
    '{', /* 0 */
    '}', /* 1 */
    '(', /* 2 */
    ')', /* 3 */
    '[', /* 4 */
    ']', /* 5 */
    '.', /* 6 */
    ',', /* 7 */
    ';', /* 8 */
    '+', /* 9 */
    '-', /* 10 */
    '*', /* 11 */
    '/', /* 12 */
    '&', /* 13 */
    '|', /* 14 */
    '<', /* 15 */
    '>', /* 16 */
    '=', /* 17 */
    '~', /* 18 */
};

const char *SEGMENT[SEGMENT_COUNT] = {
    "constant",
    "argument",
    "local",
    "static",
    "this",
    "that",
    "pointer",
    "temp",
};

const char *COMMAND[COMMAND_COUNT] = {
    "add",
    "sub",
    "neg",
    "eq",
    "gt",
    "lt",
    "and",
    "or",
    "not",
};

int LABEL_NUM = 0;

int main(int argc, char **argv)
{
    char *file_path = NULL;
    struct stat fi;

    if (argc != 2)
    {
        fprintf(stderr, "%s\n", strerror(EINVAL));
        exit(EINVAL);
    }
    file_path = argv[1];
    if (stat(file_path, &fi))
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENONET);
    }
    switch (fi.st_mode & S_IFMT)
    {
    case S_IFREG:
        open_file(file_path);
        break;
    case S_IFDIR:
        open_dir(file_path);
        break;
    default:
        fprintf(stderr, "Invalid file or directory\n");
        exit(1);
    }
    return (0);
}

void open_file(char *file_path)
{
    jack_analyzer(file_path);
}

void open_dir(char *dir_path)
{
    DIR *dp = NULL;
    struct dirent *dir;
    char path_buf[PATH_MAX];

    dp = opendir(dir_path);
    if (dp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENOENT);
    }
    for (dir = readdir(dp); dir != NULL; dir = readdir(dp))
    {
        if (dir->d_name[0] == '.')
            continue;
        switch (dir->d_type)
        {
        case DT_REG:
            if (match_extension(dir->d_name, ".jack"))
            {
                memset(path_buf, '\0', PATH_MAX);
                if (dir_path[strlen(dir_path) - 1] == '/')
                    strcat(path_buf, dir_path);
                else
                {
                    strcat(path_buf, dir_path);
                    strcat(path_buf, "/");
                }
                strcat(path_buf, dir->d_name);
                open_file(path_buf);
            }
            break;
        case DT_DIR:
            memset(path_buf, '\0', PATH_MAX);
            if (dir_path[strlen(dir_path) - 1] == '/')
                strcat(path_buf, dir_path);
            else
            {
                strncat(path_buf, dir_path, strlen(dir_path));
                strcat(path_buf, "/");
            }
            strcat(path_buf, dir->d_name);
            open_dir(path_buf);
            break;
        }
    }
    closedir(dp);
}

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
    tokens = jack_tokenizer(fp);
    fclose(fp);
    if (tokens == NULL)
    {
        fprintf(stderr, "%s:%s\n", jack_file_path, strerror(ENOMEM));
        return;
    }
    tmp = remove_extension(jack_file_path);
    if (tmp == NULL)
    {
        fprintf(stderr, "%s:%s\n", jack_file_path, strerror(ENOMEM));
        return;
    }
    file_name = add_extension(tmp, FILE_EXT);
    free(tmp);
    if (file_name == NULL)
    {
        fprintf(stderr, "%s:%s\n", jack_file_path, strerror(ENOMEM));
        return;
    }
    op = fopen(file_name, "w");
    if (op == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENONET);
    }
    free(file_name);
    compilation_engine(op, tokens, jack_file_path);
    fclose(op);
    free_token(tokens);
}
