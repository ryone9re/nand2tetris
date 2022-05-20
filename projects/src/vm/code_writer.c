#include "hack_vm.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Code writer module */
/* Write translated vm commands to asm file */
void code_writer(char *file_name, char *dest_name, vm_command *vm_commands)
{
    char *dest_file_name = NULL;
    char *symbol = NULL;
    FILE *fp = NULL;
    int vm_cmd_len = vm_command_len(vm_commands);
    int label_num = 1;

    dest_file_name = create_dest_file_name(dest_name, "/dest.asm");
    symbol = strrchr(file_name, '/');
    fp = fopen(dest_file_name, "a");
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
    free(dest_file_name);
}

/* Return directory path (need free after used) */
char *dir_path(char *file_name)
{
    char *sp = NULL;
    char *rp = NULL;
    int dir_path_len = 0;

    sp = strrchr(file_name, '/');
    dir_path_len = sp - file_name;
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
char *create_dest_file_name(char *file_name, char *ext)
{
    char *dp = NULL;
    char *fp = NULL;

    dp = dir_path(file_name);
    fp = add_ext(dp, ext);
    free(dp);
    return (fp);
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
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@LABEL%d\n", label_num);
        fprintf(fp, "D;JEQ\n");
        fprintf(fp, "D=0\n");
        fprintf(fp, "@LABEL%d\n", ++label_num);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(LABEL%d)\n", --label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "D=M-1\n");
        fprintf(fp, "A=D-1\n");
        fprintf(fp, "M=-1\n");
        fprintf(fp, "(LABEL%d)\n", ++label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        label_num++;
    }
    else if (strcmp(cmd.command, "gt") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@LABEL%d\n", label_num);
        fprintf(fp, "D;JGT\n");
        fprintf(fp, "D=0\n");
        fprintf(fp, "@LABEL%d\n", ++label_num);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(LABEL%d)\n", --label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "D=M-1\n");
        fprintf(fp, "A=D-1\n");
        fprintf(fp, "M=-1\n");
        fprintf(fp, "(LABEL%d)\n", ++label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
        label_num++;
    }
    else if (strcmp(cmd.command, "lt") == 0)
    {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@LABEL%d\n", label_num);
        fprintf(fp, "D;JLT\n");
        fprintf(fp, "D=0\n");
        fprintf(fp, "@LABEL%d\n", ++label_num);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(LABEL%d)\n", --label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "D=M-1\n");
        fprintf(fp, "A=D-1\n");
        fprintf(fp, "M=-1\n");
        fprintf(fp, "(LABEL%d)\n", ++label_num);
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M-1\n");
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
    fprintf(fp, "(%s)\n", cmd.arg1);
}

/* Write goto command */
void write_goto(vm_command cmd, FILE *fp)
{
    fprintf(fp, "@%s\n", cmd.arg1);
    fprintf(fp, "0;JMP\n");
}

/* Write if command */
void write_if(vm_command cmd, FILE *fp)
{
    fprintf(fp, "@SP\n");
    fprintf(fp, "M=M-1\n");
    fprintf(fp, "A=M\n");
    fprintf(fp, "D=M\n");
    fprintf(fp, "@%s\n", cmd.arg1);
    fprintf(fp, "D;JNE\n");
}

/* Write function command */
void write_function(vm_command cmd, FILE *fp)
{
    (void)cmd;
    (void)fp;
}

/* Write return command */
void write_return(vm_command cmd, FILE *fp)
{
    (void)cmd;
    (void)fp;
}

/* Write call command */
void write_call(vm_command cmd, FILE *fp)
{
    (void)cmd;
    (void)fp;
}
