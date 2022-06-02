#include "jack_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_extension(char *str)
{
    char *dot = NULL;
    char *dest = NULL;
    int str_len = 0;
    int ext_len = 0;
    int i = 0;

    dot = strrchr(str, '.');
    if (dot == NULL)
        return (NULL);
    str_len = strlen(str);
    ext_len = strlen(dot);
    dest = (char *)malloc(sizeof(char) * (str_len - ext_len) + sizeof(char));
    if (dest == NULL)
        return (NULL);
    while (i < (str_len - ext_len))
    {
        dest[i] = str[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}

int match_extension(char *target, char *src)
{
    char *dot = NULL;

    dot = strrchr(target, '.');
    if (strcmp(dot, src) == 0)
        return (1);
    return (0);
}

char *add_extension(char *file_name_no_ext, char *extension)
{
    int f_len = 0;
    int ext_len = 0;
    char *file_name = NULL;
    int i = 0;

    f_len = strlen(file_name_no_ext);
    if (f_len == 0)
        return (NULL);
    ext_len = strlen(extension);
    if (ext_len == 0)
        return (NULL);
    file_name = (char *)malloc(sizeof(char) * (f_len + ext_len) + sizeof(char));
    if (file_name == NULL)
        return (NULL);
    while (i < (f_len))
    {
        file_name[i] = file_name_no_ext[i];
        i++;
    }
    while (i < (f_len + ext_len))
    {
        file_name[i] = extension[i - f_len];
        i++;
    }
    file_name[i] = '\0';
    return (file_name);
}

void free_token(Token *tokens)
{
    Token *tp = NULL;
    Token *pp = NULL;

    while (1)
    {
        tp = tokens;
        pp = tokens;
        while (tp->next != NULL)
        {
            pp = tp;
            tp = tp->next;
        }
        pp->next = NULL;
        if (tp == tokens)
        {
            free(tp);
            break;
        }
        else
            free(tp);
    }
}
