#ifndef VM_WRITER_H
#define VM_WRITER_H

#include "definition.h"
#include <stdio.h>

/**
 * @brief
 * Write push command.
 * @param op
 * @param seg
 * @param index
 */
void write_push(FILE *op, enum Segment seg, int index);

/**
 * @brief
 * Write pop command.
 * @param op
 * @param seg
 * @param index
 */
void write_pop(FILE *op, enum Segment seg, int index);

/**
 * @brief
 * Write arithmetic.
 * @param op
 * @param cm
 */
void write_arithmetic(FILE *op, enum Command cm);

/**
 * @brief
 * Write label.
 * @param op
 * @param label
 */
void write_label(FILE *op, char *label);

/**
 * @brief
 * Write goto command.
 * @param op
 * @param label
 */
void write_goto(FILE *op, char *label);

/**
 * @brief
 * Write if-goto command.
 * @param op
 * @param label
 */
void write_if(FILE *op, char *label);

/**
 * @brief
 * Write call command.
 * @param op
 * @param class_name
 * @param subroutine_name
 * @param nArgs
 */
void write_call(FILE *op, char *class_name, char *subroutine_name, int nArgs);

/**
 * @brief
 * Write function command.
 * @param op
 * @param class_name
 * @param subroutine_name
 * @param nLocals
 */
void write_function(FILE *op, char *class_name, char *subroutine_name, int nLocals);

/**
 * @brief
 * Write return command.
 * @param op
 */
void write_return(FILE *op);

/**
 * @brief
 * Check arithmetic command of op.
 * @param op
 * @return enum Command
 */
enum Command check_binary_op_command(char *op);

/**
 * @brief
 * Check arithmetic command of op.
 * @param op
 * @return enum Command
 */
enum Command check_unary_op_command(char *op);

#endif /* VM_WRITER_H */
