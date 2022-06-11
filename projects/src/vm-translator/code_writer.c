#include "hack_vm.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* Code writer module */
/* Write translated vm commands to asm file */
void code_writer(char *file_name, char *dest_name, vm_command *vm_commands)
{
    char *symbol = NULL;
    FILE *fp = NULL;
    int vm_cmd_len = vm_command_len(vm_commands);
    int label_num = 1;

    symbol = strrchr(file_name, '/');
    fp = fopen(dest_name, "a");
    write_init(fp);
    for (int i = 0; i < vm_cmd_len; i++)
    {
        if (vm_commands[i].C_ARITHMETIC)
        {
            label_num = write_arithmetic(vm_commands[i], fp, label_num);
        }
        else if (vm_commands[i].C_PUSH)
        {
            write_push(vm_commands[i], fp, symbol);
        }
        else if (vm_commands[i].C_POP)
        {
            write_pop(vm_commands[i], fp, symbol);
        }
        else if (vm_commands[i].C_LABEL)
        {
            write_label(vm_commands[i], fp);
        }
        else if (vm_commands[i].C_GOTO)
        {
            write_goto(vm_commands[i], fp);
        }
        else if (vm_commands[i].C_IF)
        {
            write_if(vm_commands[i], fp);
        }
        else if (vm_commands[i].C_FUNCTION)
        {
            write_function(vm_commands[i], fp);
        }
        else if (vm_commands[i].C_RETURN)
        {
            write_return(vm_commands[i], fp);
        }
        else if (vm_commands[i].C_CALL)
        {
            write_call(vm_commands[i], fp);
        }
    }
    fclose(fp);
}

/* Return directory path (need free after used) */
char *dir_path(char *file_name)
{
    char *sp = NULL;
    char *rp = NULL;
    int dir_path_len = 0;

    sp = strrchr(file_name, '/');
    dir_path_len = sp - file_name + 1;
    rp = (char *)malloc(sizeof(char) * dir_path_len + sizeof(char));
    if (rp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }
    for (int i = 0; i < dir_path_len; i++)
        rp[i] = file_name[i];
    rp[dir_path_len] = '\0';
    return (rp);
}

/* Add ext to directory path (need free after used) */
char *add_ext(char *dir_path, char *ext)
{
    int dir_path_len = strlen(dir_path);
    int ext_len = strlen(ext);
    char *sp = NULL;
    int i = 0;

    sp = (char *)malloc(sizeof(char) * (dir_path_len + ext_len) + sizeof(char));
    if (sp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }
    while (dir_path[i] != '\0')
    {
        sp[i] = dir_path[i];
        i++;
    }
    for (int j = 0; j < ext_len; j++)
        sp[i++] = ext[j];
    sp[i] = '\0';
    return (sp);
}

/* Create dest file name (need free after used) */
char *create_dest_file_name(char *dest_name, char *file_name, unsigned int st_mode)
{
    char *dp = NULL;
    char *start = NULL;
    char *end = NULL;
    char *fp = NULL;
    char *base = NULL;
    char *file = NULL;
    int i = 0;

    dp = dir_path(dest_name);
    if (file_name[strlen(file_name) - 1] == '/')
        file_name[strlen(file_name) - 1] = '\0';
    start = strrchr(file_name, '/');
    start++;
    if (S_ISDIR(st_mode))
        end = &(file_name[strlen(file_name) - 1]);
    else
    {
        end = strrchr(file_name, '.');
        if (end == NULL)
            end = &(file_name[strlen(file_name)]);
        end--;
    }
    fp = (char *)malloc(sizeof(char) * (end - start + 1) + sizeof(char));
    while (i < (end - start + 1))
    {
        fp[i] = start[i];
        i++;
    }
    fp[i] = '\0';
    base = add_ext(dp, fp);
    free(dp);
    free(fp);
    file = add_ext(base, ".asm");
    free(base);
    return (file);
}

/* Write Initial */
void write_init(FILE *fp)
{
    fprintf(fp, "@%d\n", STACK_START_P);
    fprintf(fp, "D=A\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@%s\n", SYS_INIT);
    fprintf(fp, "0;JMP\n");
}

/* Write arithmetic command */
int write_arithmetic(vm_command cmd, FILE *fp, int label_num)
{
    if (strcmp(cmd.command, "add") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=D+M\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
    }
    else if (strcmp(cmd.command, "sub") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=M-D\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
    }
    else if (strcmp(cmd.command, "neg") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=-M\n");
    }
    else if (strcmp(cmd.command, "eq") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "@%s$LABEL%d\n", cmd.func, label_num);
        fprintf(fp, "D;JEQ\n");
        fprintf(fp, "D=0\n");
        fprintf(fp, "@%s$LABEL%d\n", cmd.func, ++label_num);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(%s$LABEL%d)\n", cmd.func, --label_num);
        fprintf(fp, "D=-1\n");
        fprintf(fp, "(%s$LABEL%d)\n", cmd.func, ++label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "M=D\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M+1\n");
        label_num++;
    }
    else if (strcmp(cmd.command, "gt") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "@%s$LABEL%d\n", cmd.func, label_num);
        fprintf(fp, "D;JGT\n");
        fprintf(fp, "D=0\n");
        fprintf(fp, "@%s$LABEL%d\n", cmd.func, ++label_num);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(%s$LABEL%d)\n", cmd.func, --label_num);
        fprintf(fp, "D=-1\n");
        fprintf(fp, "(%s$LABEL%d)\n", cmd.func, ++label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "M=D\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M+1\n");
        label_num++;
    }
    else if (strcmp(cmd.command, "lt") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "@%s$LABEL%d\n", cmd.func, label_num);
        fprintf(fp, "D;JLT\n");
        fprintf(fp, "D=0\n");
        fprintf(fp, "@%s$LABEL%d\n", cmd.func, ++label_num);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(%s$LABEL%d)\n", cmd.func, --label_num);
        fprintf(fp, "D=-1\n");
        fprintf(fp, "(%s$LABEL%d)\n", cmd.func, ++label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "M=D\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M+1\n");
        label_num++;
    }
    else if (strcmp(cmd.command, "and") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=D&M\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
    }
    else if (strcmp(cmd.command, "or") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=D|M\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
    }
    else if (strcmp(cmd.command, "not") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=!M\n");
    }
    else
    {
        fclose(fp);
        fprintf(stderr, "Syntax error!!\n");
        exit(1);
    }
    return (label_num);
}

/* Write push command */
void write_push(vm_command cmd, FILE *fp, char *file_name)
{
    size_t i = 0;
    char *symbol = (char *)malloc(sizeof(char) * (strlen(file_name) - 4) + sizeof(char));
    if (symbol == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }

    while (i < (strlen(file_name) - 4))
    {
        symbol[i] = file_name[i + 1];
        i++;
    }
    symbol[i] = '\0';
    fprintf(fp, "@%d\n", cmd.arg2);
    fprintf(fp, "D=A\n");
    if (strcmp(cmd.arg1, "argument") == 0)
    {
        fprintf(fp, "@ARG\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=M\n");
    }
    else if (strcmp(cmd.arg1, "local") == 0)
    {
        fprintf(fp, "@LCL\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=M\n");
    }
    else if (strcmp(cmd.arg1, "static") == 0)
    {
        fprintf(fp, "@%s.%d\n", symbol, cmd.arg2);
        fprintf(fp, "D=M\n");
    }
    else if (strcmp(cmd.arg1, "constant") == 0)
    {
        /* Nothing to do */
    }
    else if (strcmp(cmd.arg1, "this") == 0)
    {
        fprintf(fp, "@THIS\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=M\n");
    }
    else if (strcmp(cmd.arg1, "that") == 0)
    {
        fprintf(fp, "@THAT\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=M\n");
    }
    else if (strcmp(cmd.arg1, "pointer") == 0)
    {
        fprintf(fp, "@%d\n", 3 + cmd.arg2);
        fprintf(fp, "D=M\n");
    }
    else if (strcmp(cmd.arg1, "temp") == 0)
    {
        fprintf(fp, "@%d\n", 5 + cmd.arg2);
        fprintf(fp, "D=M\n");
    }
    fprintf(fp, "@SP\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M+1\n");
    free(symbol);
}

/* Write pop command */
void write_pop(vm_command cmd, FILE *fp, char *file_name)
{
    size_t i = 0;
    char *symbol = (char *)malloc(sizeof(char) * (strlen(file_name) - 4) + sizeof(char));
    if (symbol == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }

    while (i < (strlen(file_name) - 4))
    {
        symbol[i] = file_name[i + 1];
        i++;
    }
    fprintf(fp, "@%d\n", cmd.arg2);
    fprintf(fp, "D=A\n");
    if (strcmp(cmd.arg1, "argument") == 0)
    {
        fprintf(fp, "@ARG\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=A\n");
    }
    else if (strcmp(cmd.arg1, "local") == 0)
    {
        fprintf(fp, "@LCL\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=A\n");
    }
    else if (strcmp(cmd.arg1, "static") == 0)
    {
        fprintf(fp, "@%s.%d\n", symbol, cmd.arg2);
        fprintf(fp, "D=A\n");
    }
    else if (strcmp(cmd.arg1, "constant") == 0)
    {
        /* Nothing to do */
    }
    else if (strcmp(cmd.arg1, "this") == 0)
    {
        fprintf(fp, "@THIS\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=A\n");
    }
    else if (strcmp(cmd.arg1, "that") == 0)
    {
        fprintf(fp, "@THAT\n");
        fprintf(fp, "A=D+M\n");
        fprintf(fp, "D=A\n");
    }
    else if (strcmp(cmd.arg1, "pointer") == 0)
    {
        fprintf(fp, "@%d\n", 3 + cmd.arg2);
        fprintf(fp, "D=A\n");
    }
    else if (strcmp(cmd.arg1, "temp") == 0)
    {
        fprintf(fp, "@%d\n", 5 + cmd.arg2);
        fprintf(fp, "D=A\n");
    }
    fprintf(fp, "@R13\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@R13\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
}

/* Write label command */
void write_label(vm_command cmd, FILE *fp)
{
    if (cmd.func != NULL)
        fprintf(fp, "(%s$%s)\n", cmd.func, cmd.arg1);
    else
        fprintf(fp, "(%s)\n", cmd.arg1);
}

/* Write goto command */
void write_goto(vm_command cmd, FILE *fp)
{
    fprintf(fp, "@%s$%s\n", cmd.func, cmd.arg1);
    fprintf(fp, "0;JMP\n");
}

/* Write if command */
void write_if(vm_command cmd, FILE *fp)
{
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@%s$%s\n", cmd.func, cmd.arg1);
    fprintf(fp, "D;JNE\n");
}

/* Write function command */
void write_function(vm_command cmd, FILE *fp)
{
    fprintf(fp, "(%s)\n", cmd.arg1);
    for (int i = 0; i < cmd.arg2; i++)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M+1\n");
    }
}

// おそらくここがおかしい
/* Write return command */
void write_return(vm_command cmd, FILE *fp)
{
    (void)cmd;
    fprintf(fp, "@LCL\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@R13\n");
    fprintf(fp, "M=D\n");
    for (int i = 0; i < 5; i++)
        fprintf(fp, "D=D-1\n");
    fprintf(fp, "A=D\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@R14\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@ARG\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@ARG\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=D+1\n");
    fprintf(fp, "@R13\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@THAT\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@R13\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@THIS\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@R13\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@ARG\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@R13\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@LCL\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@R14\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "0;JMP\n");
}

/* Write call command */
void write_call(vm_command cmd, FILE *fp)
{
    fprintf(fp, "@%s_%d\n", cmd.arg1, cmd.count + 1);
    fprintf(fp, "D=A\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M+1\n");
    fprintf(fp, "@LCL\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M+1\n");
    fprintf(fp, "@ARG\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M+1\n");
    fprintf(fp, "@THIS\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M+1\n");
    fprintf(fp, "@THAT\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M+1\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "D=M\n");
    for (int i = 0; i < (cmd.arg2 + 5); i++)
        fprintf(fp, "D=D-1\n");
    fprintf(fp, "@ARG\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@SP\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@LCL\n");
    fprintf(fp, "M=D\n");
    fprintf(fp, "@%s\n", cmd.arg1);
    fprintf(fp, "0;JMP\n");
    fprintf(fp, "(%s_%d)\n", cmd.arg1, cmd.count + 1);
}
