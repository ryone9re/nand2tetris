#include "jack_compiler.h"
#include <stdio.h>

void write_push(FILE *op, enum Segment seg, int index)
{
    fprintf(op, "push %s %d\n", SEGMENT[seg], index);
}

void write_pop(FILE *op, enum Segment seg, int index)
{
    fprintf(op, "pop %s %d\n", SEGMENT[seg], index);
}

void write_arithmetic(FILE *op, enum Command cm)
{
    fprintf(op, "%s\n", COMMAND[cm]);
}

void write_label(FILE *op, char *label)
{
    fprintf(op, "label %s\n", label);
}

void write_goto(FILE *op, char *label)
{
    fprintf(op, "goto %s\n", label);
}

void write_if(FILE *op, char *label)
{
    fprintf(op, "if-goto %s\n", label);
}

void write_call(FILE *op, char *class_name, char *subroutine_name, int nArgs)
{
    fprintf(op, "call %s.%s %d\n", class_name, subroutine_name, nArgs);
}

void write_function(FILE *op, char *class_name, char *subroutine_name, int nLocals)
{
    fprintf(op, "function %s.%s %d\n", class_name, subroutine_name, nLocals);
}

void write_return(FILE *op)
{
    fprintf(op, "return\n");
}

enum Command check_binary_op_command(char *op)
{
    if (op[0] == SYMBOLS[9])
        return (ADD);
    if (op[0] == SYMBOLS[10])
        return (SUB);
    if (op[0] == SYMBOLS[13])
        return (AND);
    if (op[0] == SYMBOLS[14])
        return (OR);
    if (op[0] == SYMBOLS[15])
        return (LT);
    if (op[0] == SYMBOLS[16])
        return (GT);
    return (EQ);
}

enum Command check_unary_op_command(char *op)
{
    if (op[0] == SYMBOLS[10])
        return (NEG);
    return (NOT);
}
