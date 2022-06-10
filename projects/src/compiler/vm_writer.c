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
