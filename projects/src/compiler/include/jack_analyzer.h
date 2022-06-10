#ifndef JACK_ANALYZER_H
#define JACK_ANALYZER_H

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

/**
 * @brief
 * Open jack file and prepare writing file.
 * @param jack_file_name
 * Jack File path.
 */
void jack_analyzer(char *jack_file_path);

#endif /* JACK_ANALYZER_H */
