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
    char *word;
    Token *next;
    enum Token_type token_type;
};

#define KEYWORD_COUNT 21
#define SYMBOL_COUNT 19

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
 * @return Token* Tokenized data array.
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
Token *add_token(Token *tokens, char *word, enum Token_type token_type);

/* compilation_engine.c */

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

#endif /* JACK_COMPILER_H */
