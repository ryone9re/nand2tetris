#include "assembler.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Return row count in buffer */
int row_counter(char *buffer)
{
    int count = 0;

    while (*buffer != '\0')
    {
        if (*buffer == '\n' || *buffer == '\0')
            count++;
        buffer++;
    }
    return (count);
}

/* Return line length in buffer from start_word number */
int get_line_length(char *buffer, int start_word)
{
    int count = 0;
    char *start_point = buffer + start_word;

    while (*start_point != '\r' && *start_point != '\n' && *start_point != '\0')
    {
        count++;
        start_point++;
    }
    if (count == 0)
        return (EOF);
    return (count);
}

/* Slice buffer until now_word_num to line_length (need free after used) */
char *word_slicer(char *buffer, int now_word_num, int line_length)
{
    int i = 0;
    char *start_point = buffer + now_word_num;
    char *sliced_word = (char *)malloc(sizeof(char) * line_length + sizeof(char));

    while (start_point[i] != '\0' && i < line_length)
    {
        sliced_word[i] = start_point[i];
        i++;
    }
    sliced_word[i] = '\0';
    return (sliced_word);
}

/* Initialize buffer */
void buffer_initializer(char *buffer)
{
    while (*buffer != '\0')
        *buffer++ = '\0';
}

/* Return file name that ext is new_ext (need free after used) */
char *file_ext_converter(char *src, char *new_ext)
{
    int src_length = 0;
    int new_ext_length = 0;
    char *ext_start_point = NULL;
    char *file_name_noext = NULL;
    char *new_file_name = NULL;
    char *new_file_name_p = NULL;

    ext_start_point = strrchr(src, '.');
    file_name_noext = word_slicer(src, 0, ext_start_point - src);
    src_length = strlen(file_name_noext);
    new_ext_length = strlen(new_ext);
    new_file_name = (char *)malloc(sizeof(char) * (src_length + new_ext_length) + sizeof(char));
    new_file_name_p = new_file_name;
    while (*file_name_noext != '\0')
        *new_file_name_p++ = *file_name_noext++;
    while (*new_ext != '\0')
        *new_file_name_p++ = *new_ext++;
    *new_file_name_p = '\0';
    return (new_file_name);
}

/* Open/Create file and append word in output */
void write_output(char *file_name, char *ext, char *output)
{
    char *output_file_name = NULL;
    FILE *fp;

    output_file_name = file_ext_converter(file_name, ext);
    fp = fopen(output_file_name, "a");
    fprintf(fp, "%s\n", output);
    fclose(fp);
    free(output_file_name);
}

/* Set given string to buffer */
void set_buffer(char *buffer, char *word, int size)
{
    char *bp = buffer;
    char *wp = word;

    for (int i = 0; i < size; i++)
        *bp++ = *wp++;
}

/* Return if string is number */
int isnumber(char *str)
{
    while (*str != '\0')
    {
        if (isdigit(*str) == FALSE)
            return (FALSE);
        str++;
    }
    return (TRUE);
}
