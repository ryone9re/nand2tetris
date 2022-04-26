#include "assembler.h"
#include <stdlib.h>
#include <stdio.h>

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

    while (*start_point != '\n' && *start_point != '\0')
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
