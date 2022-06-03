#ifndef JACK_COMPILER_H
#define JACK_COMPILER_H

/* includes */
#include <stdio.h>

/* defenition */
enum Token_type
{
    NOTOKEN,
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
};

typedef struct Token Token;

struct Token
{
    int row;
    char *word;
    Token *next;
    enum Token_type token_type;
};

#define KEYWORD_COUNT 21
#define SYMBOL_COUNT 19

#define MIN_INT 0
#define MAX_INT 32767

/* globals */
extern const char *KEYWORDS[KEYWORD_COUNT];
extern const char SYMBOLS[SYMBOL_COUNT];

/* main.c */

/**
 * @brief
 * Open file and pass to the analyzer.
 * @param file_path
 * File path.
 */
void open_file(char *file_path);

/**
 * @brief
 * Open the directory and recursively call functions on its contents.
 * @param dir_path
 * Direcroty path.
 */
void open_dir(char *dir_path);

/* jack_analyzer.c */

/**
 * @brief
 * Open jack file and prepare writing file.
 * @param jack_file_name
 * Jack File path.
 */
void jack_analyzer(char *jack_file_path);

/* jack_tokenizer.c */

/**
 * @brief
 * Tokenize from given file pointer.
 * @param fp Target file pointer.
 * @return Token* Tokens.
 */
Token *jack_tokenizer(FILE *fp);

/**
 * @brief
 * Check str is in string constant.
 * @param str Target string.
 * @return int Return 1 if it's in string constant, else 0.
 */
int is_instr(char *str);

/**
 * @brief
 * Check str is space.
 * @param str Target string.
 * @return int Return 1 if it's spaces, else 0.
 */
int is_spaces(char *str);

/**
 * @brief
 * Check str is keyword.
 * @param str Target string.
 * @return int Return 1 if it's keyword, else 0.
 */
int is_keyword(char *str);

/**
 * @brief
 * Check token is keyword constant.
 * @param str Target string.
 * @return int If true return 1, else 0.
 */
int is_keyword_constant(char *str);

/**
 * @brief
 * Check character is symbol.
 * @param c Target character.
 * @return int Return 1 if it's symbol, else 0.
 */
int is_symbol(char c);

/**
 * @brief
 * Check str is symbol.
 * @param c Target string.
 * @return int Return 1 if it's symbol, else 0.
 */
int is_symbol_str(char *str);

/**
 * @brief
 * Check str is integer constant.
 * @param str Target string.
 * @return int Return 1 if it's symbol, else 0.
 */
int is_integer_constant(char *str);

/**
 * @brief
 * Check str is string constant.
 * @param str Target string.
 * @return int Return 1 if it's symbol, else 0.
 */
int is_string_constant(char *str);

/**
 * @brief
 * Check str is identifier.
 * @param str Target string.
 * @return int Return 1 if it's symbol, else 0.
 */
int is_identifier(char *str);

/**
 * @brief
 * Generated new Token at the end of token.
 * @param tokens Target tokens.
 * @return Token* Return new token address.
 */
Token *new_token(Token *tokens);

/**
 * @brief
 * Set word and token type to the end of token.
 * @param tokens Target tokens.
 * @param word Token word want to add.
 * @param token_type Token type want to add.
 * @return Token* Return token address set new token.
 */
Token *add_token(int row, Token *tokens, char *word, enum Token_type token_type);

/* compilation_engine.c */

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
 * @return Token* Next token.
 */
Token *write_keyword(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Write symbol.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *write_symbol(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Write integer constant.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *write_integer_constant(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Write string constant.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *write_string_constant(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Write identifier.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *write_identifier(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile subroutine call.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_subroutine_call(FILE *op, Token *tokens, char *file_name);

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
 * @return Token* Next token.
 */
Token *compile_class_var_dec(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Write type.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_type(FILE *op, Token *tokens, char *file_name);

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
 * @return Token* Next token.
 */
Token *compile_subroutine_dec(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile parameter list without "()".
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_parameter_list(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile method, function and constructor body.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_subroutine_body(FILE *op, Token *token, char *file_name);

/**
 * @brief
 * Compile var declaration.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_var_dec(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile statements without "{}".
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_statements(FILE *op, Token *tokens, char *file_name);

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
 * @return Token* Next token.
 */
Token *compile_do(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_let(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile while statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_while(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile return statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_return(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile if statement, possibly bring with else statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_if(FILE *op, Token *tokens, char *file_name);

/**
 * @brief
 * Compile expression.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @return Token* Next token.
 */
Token *compile_expression(FILE *op, Token *tokens, char *file_name);

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
 * @return Token* Next token.
 */
Token *compile_term(FILE *op, Token *tokens, char *file_name);

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
 * @return Token* Next token.
 */
Token *compile_expression_list(FILE *op, Token *tokens, char *file_name);

/* utils.c */

/**
 * @brief
 * Remove extension from str.
 * @param str
 * File path
 * @return char* (Need free after used!)
 * If str is containing extension, return str without extension.
 * Else return NULL.
 */
char *remove_extension(char *str);

/**
 * @brief
 * Compare target extension matchs src.
 * @param target
 * Comparison target.
 * @param src
 * Comparison source.
 * @return int
 * If matched return 1, else return 0.
 */
int match_extension(char *target, char *src);

/**
 * @brief
 * Add extension to file_name_no_ext.
 * @param file_name_no_ext
 * @param extension
 * @return char* (Need free after used!)
 */
char *add_extension(char *file_name_no_ext, char *extension);

/**
 * @brief
 * Free all tokens.
 * @param tokens Token pointer.
 */
void free_token(Token *tokens);

#endif /* JACK_COMPILER_H */
