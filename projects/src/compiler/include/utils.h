#ifndef UTILS_H
#define UTILS_H

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

#endif /* UTILS_H */
