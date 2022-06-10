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

enum Kind
{
    Static,
    Field,
    Argument,
    Var
};

typedef struct Symbol Symbol;

struct Symbol
{
    char *name;
    char *type;
    enum Kind kind;
    int index;
    Symbol *next;
};

typedef struct Symbol_table Symbol_table;

struct Symbol_table
{
    Symbol *class;
    Symbol *subroutine;
};

enum Segment
{
    CONST,
    ARG,
    LOCAL,
    STATIC,
    THIS,
    THAT,
    POINTER,
    TEMP
};

enum Command
{
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT
};

#define FILE_EXT ".xml"

#define KEYWORD_COUNT 21
#define SYMBOL_COUNT 19
#define SEGMENT_COUNT 8
#define COMMAND_COUNT 9

#define MIN_INT 0
#define MAX_INT 32767

/* globals */
extern const char *KEYWORDS[KEYWORD_COUNT];
extern const char SYMBOLS[SYMBOL_COUNT];
extern const char *SEGMENT[SEGMENT_COUNT];
extern const char *COMMAND[COMMAND_COUNT];

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

/* symbol_table.c */

/**
 * @brief Need free!
 * Initialize symbol table.
 * @return Symbol_table* Initialized symbol table.
 */
Symbol_table *initialize_symbol_table();

/**
 * @brief
 * Free symbol table
 * @param symbol_table
 */
void free_symbol_table(Symbol_table *symbol_table);

/**
 * @brief
 * Create new symbol.
 * @return Symbol* Created symbol.
 */
Symbol *new_symbol();

/**
 * @brief
 * Free symbols.
 * @param symbols Symbol list
 */
void free_symbol(Symbol *symbols);

/**
 * @brief
 * Add new symbol to tale of symbol.
 * @param symbols
 * @param new
 * @return Symbol* Head of symbol.
 */
Symbol *add_symbol(Symbol *symbol, Symbol *new);

/**
 * @brief
 * Count kind number in symbol_table.
 * @param symbol_table
 * @param kind
 * @return int
 */
int var_count(Symbol_table *symbol_table, enum Kind kind);

/* vm_writer.c */

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
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_subroutine_call(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

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
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_subroutine_body(FILE *op, Token *token, char *file_name, Symbol_table *symbol_table);

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
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_statements(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

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
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_do(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_let(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile while statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_while(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile return statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_return(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile if statement, possibly bring with else statement.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_if(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

/**
 * @brief
 * Compile expression.
 * @param op Export file pointer.
 * @param tokens Tokens.
 * @param file_name Opening file name.
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_expression(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

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
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_term(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

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
 * @param symbol_table Symbol table.
 * @return Token* Next token.
 */
Token *compile_expression_list(FILE *op, Token *tokens, char *file_name, Symbol_table *symbol_table);

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
