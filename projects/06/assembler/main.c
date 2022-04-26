#include "assembler.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Buffer containing only instructions */
char buffer[MAXSIZE];

/* Buffer for output */
char output[17];

int main(int argc, char **argv)
{
    (void)argc;
    char *file_name = argv[1];

    parser(file_name);
    return (0);
}

/* Parser module */
void parser(char *file_name)
{
    char **instructions;

    constructor(file_name);
    instructions = buffer_splitter(buffer);
    for (int i = 0; instructions[i] != NULL; i++)
    {
        // 各単語のパース､バッファへ書き込み
        buffer_initializer(output);
        command_type t = check_command_type(instructions[i]);
        if (t.A_COMMAND == 1)
        {
            // A命令の処理
        }
        else if (t.L_COMMAND == 1)
        {
            // シンボルの処理
        }
        else
        {
            // C命令の処理
        }
    }
    // ファイルの最後に\0を書き込む
    free(instructions);
}

/* Parser constructor */
/* Read file and store only instructions in buffer */
void constructor(char *file_name)
{
    FILE *fp = NULL;
    char c = -1;
    int in_comment = FALSE;
    int in_space = FALSE;
    int last_lf = FALSE;

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("%s\n", strerror(ENOENT));
        exit(ENOENT);
    }
    for (int i = 0; (c = fgetc(fp)) != EOF && i < MAXSIZE;)
    {
        if (c == '/')
            in_comment = TRUE;
        if (isspace(c) != FALSE)
        {
            in_space = TRUE;
            if (last_lf == TRUE)
                in_space = FALSE;
        }
        if (in_comment == FALSE && in_space == FALSE)
        {
            buffer[i] = c;
            last_lf = TRUE;
            i++;
        }
        if (c == '\n')
        {
            in_comment = FALSE;
            in_space = FALSE;
            last_lf = FALSE;
        }
    }
    fclose(fp);
}

/* Split instructions in buffer (need free after used) */
char **buffer_splitter(char *buffer)
{
    int i = 0;
    int line_length = 0;
    int now_word_num = 0;
    int row_count = 0;
    char *sliced_word = NULL;
    char **instructions = NULL;

    row_count = row_counter(buffer);
    instructions = (char **)malloc(sizeof(char *) * row_count + sizeof(char *));
    while ((line_length = get_line_length(buffer, now_word_num)) != EOF && i < row_count)
    {
        sliced_word = word_slicer(buffer, now_word_num, line_length);
        instructions[i] = (char *)malloc(sizeof(char) * line_length + sizeof(char));
        strcpy(instructions[i], sliced_word);
        free(sliced_word);
        now_word_num += line_length + 1;
        i++;
    }
    instructions[i] = NULL;
    return (instructions);
}

/* Check command type for given word */
command_type check_command_type(char *word)
{
    command_type t = {0};
    int word_length = 0;

    word_length = strlen(word);
    if (word[0] == '@')
        t.A_COMMAND = 1;
    else if (word[0] == '(' && word[word_length - 1] == ')')
        t.L_COMMAND = 1;
    else
        t.C_COMMAND = 1;
    return (t);
}

/* Code module */
