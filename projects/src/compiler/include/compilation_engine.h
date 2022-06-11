#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H

#include "jack_compiler.h"

/**
 * @brief
 * Compilation engine.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 */
void compilation_engine(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Write keyword.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *write_keyword(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Write symbol.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *write_symbol(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Write integer constant.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *write_integer_constant(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Write string constant.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *write_string_constant(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Write identifier.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *write_identifier(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile subroutine call.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_subroutine_call(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile class.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
void compile_class(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile static declaration or field declaration.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_class_var_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Write type.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_type(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Check token is type.
 * @param token Target token.
 * @return int Return 1 if it's type, else 0.
 */
int is_type(Token *token);

/**
 * @brief
 * Compile method, function and constructor declaration.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_subroutine_dec(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile parameter list without "()".
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_parameter_list(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile method, function and constructor body.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param subroutine_name Subroutine name.
 * @param subroutine_kind Subroutine kind.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_subroutine_body(FILE *op, Token *token, char *file_name, char *class_name, char *subroutine_name, enum Subroutine_Kind subroutine_kind, Symbol_table *symbol_table);

/**
 * @brief
 * Compile var declaration.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_var_dec(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile statements without "{}".
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_statements(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Check token is statement.
 * @param token Token.
 * @return int If true return 1, else 0.
 */
int is_statement(Token *token);

/**
 * @brief
 * Compile do statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_do(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_let(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile while statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_while(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile return statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_return(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile if statement, possibly bring with else statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_if(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile expression.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_expression(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Check token is expression.
 * @param token Token.
 * @return int If true return 1, else 0.
 */
int is_expression(Token *token);

/**
 * @brief
 * Check token is op.
 * @param token Token.
 * @return int If true return 1, else 0.
 */
int is_op(Token *token);

/**
 * @brief
 * Check token is unary op.
 * @param token Token.
 * @return int If true return 1, else 0.
 */
int is_unary_op(Token *token);

/**
 * @brief
 * Compile term.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_term(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Check token is term.
 * @param token Token.
 * @return int If true return 1, else 0.
 */
int is_term(Token *token);

/**
 * @brief
 * Compile expression list.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param class_name Class name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_expression_list(FILE *op, Token *tokens, char *file_name, char *class_name, Symbol_table *symbol_table);

/**
 * @brief
 * Count expressions.
 * @param token
 * @return int
 */
int count_expression(Token *token);

#endif /* COMPILATION_ENGINE_H */
