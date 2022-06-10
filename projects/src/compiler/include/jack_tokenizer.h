#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include "definition.h"
#include <stdio.h>

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

#endif /* JACK_TOKENIZER_H */
