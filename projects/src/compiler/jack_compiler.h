#ifndef JACK_COMPILER_H
#define JACK_COMPILER_H

/* includes */
#include <stdio.h>

/* defenition */

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
 * @return char** Tokenized string array.
 */
char **jack_tokenizer(FILE *fp);

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
