#include "hack_vm.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Parser module */
/* Parsed row excluding comment and spaces and set to vm_command array */
vm_command *parser(char *buffer, vm_command *vm_commands)
{
    char *bp = buffer;
    char *command = NULL;
    int command_len = 0;
    vm_command *temp_vm_commands;

    if (is_comment_row(buffer) == 1 || is_space_row(buffer) == 1)
        return (vm_commands);
    while (isspace(*bp))
        bp++;
    remove_comment(bp);
    remove_last_space(bp);
    command = strdup(bp);
    command_len = vm_command_len(vm_commands);
    temp_vm_commands = (vm_command *)realloc(vm_commands, sizeof(vm_command) * (command_len + 1));
    if (temp_vm_commands == NULL)
    {
        free(vm_commands);
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }
    vm_commands = temp_vm_commands;
    add_to_command(vm_commands, command);
    free(command);
    return (vm_commands);
}

/* Initialize vm_command */
void initialize_vm_command(vm_command *v_command)
{
    v_command->C_ARITHMETIC = 0;
    v_command->C_PUSH = 0;
    v_command->C_POP = 0;
    v_command->C_LABEL = 0;
    v_command->C_GOTO = 0;
    v_command->C_IF = 0;
    v_command->C_FUNCTION = 0;
    v_command->C_RETURN = 0;
    v_command->C_CALL = 0;
    v_command->IS_NULL = 0;
    v_command->command = NULL;
    v_command->arg1 = NULL;
    v_command->arg2 = 0;
    v_command->func = NULL;
    v_command->count = 0;
}

/* Check src is comment (TRUE: return 1, FALSE: return 0) */
int is_comment_row(char *src)
{
    if (src[0] == '/')
        return (1);
    return (0);
}

/* Check src is only contain space (TRUE: return 1, FALSE: return 0) */
int is_space_row(char *src)
{
    int word_length = strlen(src);
    int space_count = 0;

    for (int i = 0; src[i] != '\0'; i++)
    {
        if (isspace(src[i]))
            space_count++;
    }
    if (word_length == space_count)
        return (1);
    return (0);
}

/* Remove comment block */
void remove_comment(char *word)
{
    int i = 0;
    int word_len = strlen(word);

    while (word[i] != '\0' && word[i] != '/')
        i++;
    if (word[i] == '/')
        memset(word + i, '\0', word_len - i);
}

/* Remove last wasted space */
void remove_last_space(char *word)
{
    int word_len = strlen(word);
    int i = 0;

    while (isspace(word[(word_len - 1) - i]))
    {
        if ((word_len - 1) - i == 0)
            break;
        i++;
    }
    memset(word + (word_len - i), '\0', i);
}

/* Return vm_commands array length */
int vm_command_len(vm_command *vm_commands)
{
    int i = 0;

    while (vm_commands[i].IS_NULL != 1)
        i++;
    return (i + 1);
}

/* Add command to vm_comannd array vm_commands */
void add_to_command(vm_command *vm_commands, char *command)
{
    int len = vm_command_len(vm_commands);
    vm_command *before;
    if (len > 1)
        before = &vm_commands[len - 2];
    vm_command *add_to = &vm_commands[len - 1];
    vm_command *last = &vm_commands[len];
    char *cm = pick_command(command);
    char *arg1 = pick_arg1(command);
    int arg2 = pick_arg2(command);

    initialize_vm_command(add_to);
    if (isarithmetic(cm) == 1)
    {
        add_to->C_ARITHMETIC = 1;
        add_to->command = strdup(cm);
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "push") == 0)
    {
        add_to->C_PUSH = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        add_to->arg2 = arg2;
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "pop") == 0)
    {
        add_to->C_POP = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        add_to->arg2 = arg2;
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "label") == 0)
    {
        add_to->C_LABEL = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "goto") == 0)
    {
        add_to->C_GOTO = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "if-goto") == 0)
    {
        add_to->C_IF = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "function") == 0)
    {
        add_to->C_FUNCTION = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        add_to->arg2 = arg2;
        add_to->func = strdup(arg1);
        add_to->count = len;
    }
    else if (strcmp(cm, "call") == 0)
    {
        add_to->C_CALL = 1;
        add_to->command = strdup(cm);
        add_to->arg1 = strdup(arg1);
        add_to->arg2 = arg2;
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else if (strcmp(cm, "return") == 0)
    {
        add_to->C_RETURN = 1;
        add_to->command = strdup(cm);
        if (len > 1 && before->func != NULL)
            add_to->func = strdup(before->func);
        add_to->count = len;
    }
    else
        add_to->IS_NULL = 1;
    initialize_vm_command(last);
    last->IS_NULL = 1;
    if (cm != NULL)
        free(cm);
    if (arg1 != NULL)
        free(arg1);
}

/* Pick command word from given command */
char *pick_command(char *command)
{
    int i = 0;

    while (isspace(command[i]) == 0 && command[i] != '\0')
        i++;
    return (word_slicer(command, i));
}

/* Pick arg1 from given command */
char *pick_arg1(char *command)
{
    int i = 0;
    int j = 0;
    int k = 0;

    while (isspace(command[i]) == 0 && command[i] != '\0')
        i++;
    while (isspace(command[i]))
        i++;
    j = i;
    while (isspace(command[i]) == 0 && command[i] != '\0')
    {
        i++;
        k++;
    }
    return (word_slicer(command + j, k));
}

/* Return arg2 from given command */
int pick_arg2(char *command)
{
    int i = 0;

    while (isspace(command[i]) == 0 && command[i] != '\0')
        i++;
    while (isspace(command[i]))
        i++;
    while (isspace(command[i]) == 0 && command[i] != '\0')
        i++;
    while (isspace(command[i]))
        i++;
    return (atoi(&command[i]));
}

/* Slice given words until n (need free after used) */
char *word_slicer(char *words, int n)
{
    char *str = (char *)malloc(sizeof(char) * n + sizeof(char));

    for (int i = 0; i < n; i++)
        str[i] = words[i];
    str[n] = '\0';
    return (str);
}

/* Return if word is arithmetic TRUE(1) or FALSE(0)  */
int isarithmetic(char *word)
{
    if (strcmp(word, "add") == 0)
        return (1);
    else if (strcmp(word, "sub") == 0)
        return (1);
    else if (strcmp(word, "neg") == 0)
        return (1);
    else if (strcmp(word, "eq") == 0)
        return (1);
    else if (strcmp(word, "gt") == 0)
        return (1);
    else if (strcmp(word, "lt") == 0)
        return (1);
    else if (strcmp(word, "and") == 0)
        return (1);
    else if (strcmp(word, "or") == 0)
        return (1);
    else if (strcmp(word, "not") == 0)
        return (1);
    else
        return (0);
}

/* Free commands */
void free_commands(vm_command *vm_commands)
{
    int i = vm_command_len(vm_commands);

    for (int j = i - 1; j >= 0; j--)
    {
        if (vm_commands[j].command != NULL)
            free(vm_commands[j].command);
        if (vm_commands[j].arg1 != NULL)
            free(vm_commands[j].arg1);
        if (vm_commands[j].func != NULL)
            free(vm_commands[j].func);
    }
    free(vm_commands);
}
