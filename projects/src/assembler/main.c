#include "hack_assembler.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Buffer containing only instructions */
char buffer[MAXSIZE];

/* Buffer for output */
char output[16];

int main(int argc, char **argv)
{
    (void)argc;
    char *file_name = argv[1];
    symbol_table *stp = NULL;

    stp = create_symbol_table();
    parser(file_name, stp);
    free_symbol_table(stp);
    return (0);
}

/* Parser module */
void parser(char *file_name, symbol_table *stp)
{
    char **instructions;
    char *symbol = NULL;
    int instruction_count = 0;
    int addres_i = -1;
    char resolved_symbol[READSIZ];
    int variable_count = 16;

    constructor(file_name);
    instructions = buffer_splitter(buffer);
    first_path(stp, instructions);
    for (int i = 0; instructions[i] != NULL; i++)
    {
        buffer_initializer(output);
        command_type t = check_command_type(instructions[i]);
        if (t.A_COMMAND == 1)
        {
            symbol = pick_symbol(instructions[i], t);
            if (isnumber(symbol) == TRUE)
                strcpy(resolved_symbol, symbol);
            else
            {
                if (contains(stp, symbol) == TRUE)
                {
                    addres_i = get_address(stp, symbol);
                    sprintf(resolved_symbol, "%d", addres_i);
                }
                else
                {
                    add_entry(stp, symbol, variable_count);
                    sprintf(resolved_symbol, "%d", variable_count);
                    variable_count++;
                }
            }
            free(symbol);
            a_commander(resolved_symbol, file_name);
            instruction_count++;
        }
        else if (t.C_COMMAND == 1)
        {
            c_commander(instructions[i], file_name);
            instruction_count++;
        }
    }
    deep_free(instructions);
}

/* Parser constructor */
/* Read file and store only instructions in buffer */
void constructor(char *file_name)
{
    FILE *fp = NULL;
    char c = -1;
    int in_comment = FALSE;
    int need_lf = FALSE;

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
        if (in_comment == TRUE && c != '\r' && c != '\n')
            continue;
        if (isspace(c) != FALSE && c != '\r' && c != '\n')
            continue;
        if (isgraph(c) != FALSE)
        {
            buffer[i] = c;
            need_lf = TRUE;
            i++;
        }
        if ((c == '\r' || c == '\n') && need_lf == TRUE)
        {
            buffer[i] = c;
            i++;
            in_comment = FALSE;
            need_lf = FALSE;
        }
        else if (c == '\r' || c == '\n')
        {
            in_comment = FALSE;
            need_lf = FALSE;
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

/* Free double pointer */
void deep_free(char **instructions)
{
    int i = 0;

    while (instructions[i] != NULL)
    {
        free(instructions[i]);
        i++;
    }
    free(instructions[i]);
    free(instructions);
}

/* Code module */
/* Execute command corresponding A instruction */
void a_commander(char *symbol, char *file_name)
{
    char *op = output;
    int symbol_int = 0;
    char temp[15];
    char binary_buffer[15];

    memset(temp, '0', sizeof(char) * 15);
    memset(binary_buffer, '\0', sizeof(char) * 15);
    *op++ = '0';
    symbol_int = atoi(symbol);
    /* Symbol is address so it does not take a negative value */
    for (int i = 0; i < 15 && symbol_int > 0; i++, symbol_int /= 2)
        temp[i] = (symbol_int % 2) + '0';
    for (int i = 0, j = 14; i < 15 && j >= 0; i++, j--)
        binary_buffer[i] = temp[j];
    for (int i = 0; i < 15; i++)
        op[i] = binary_buffer[i];
    write_output(file_name, ".hack", output);
}

/* Execute command corresponding C instruction */
void c_commander(char *word, char *file_name)
{
    char *op = output;
    dest_mnemonic dm = {0};
    char temp_dest[3];
    comp_mnemonic cm = {0};
    char temp_comp[7];
    jump_mnemonic jm = {0};
    char temp_jump[3];

    memset(temp_dest, '\0', sizeof(char) * 3);
    memset(temp_comp, '\0', sizeof(char) * 7);
    memset(temp_jump, '\0', sizeof(char) * 3);
    for (int i = 0; i < 3; i++)
        *op++ = '1';
    dm = pick_dest(word);
    write_dest_bin(temp_dest, dm);
    cm = pick_comp(word);
    if (cm.COMP_NULL == 1)
    {
        fprintf(stderr, "%s\n", strerror(EINVAL));
        exit(EINVAL);
    }
    write_comp_bin(temp_comp, cm);
    jm = pick_jump(word);
    write_jump_bin(temp_jump, jm);
    for (int i = 0; i < 7; i++)
        *op++ = temp_comp[i];
    for (int i = 0; i < 3; i++)
        *op++ = temp_dest[i];
    for (int i = 0; i < 3; i++)
        *op++ = temp_jump[i];
    write_output(file_name, ".hack", output);
}
