/**
 * @brief Removes all unnecessary white spaces from a file.
 *
 * This function processes the input file to remove any extra white spaces,
 * returning the name of a new file that contains the cleaned content.
 *
 * @param file_name A string representing the name of the input file.
 * @return A string with the name of the new file after white space removal.
 */
char *clean_file_spaces(char file_name[]);

/**
 * @brief Copies a specified portion of text from a file into a string.
 *
 * This function reads a specified number of characters from a file, starting
 * at a given position, and returns them as a string.
 *
 * @param fp A file pointer to the file from which to read.
 * @param pos A pointer to the position in the file where reading should begin.
 * @param length The number of characters to copy from the file.
 * @return A string containing the copied characters.
 */
char *extract_text(FILE *fp, fpos_t *pos, int length);

/**
 * @brief Removes unnecessary white spaces from a string.
 *
 * This function modifies a string by eliminating any extra white spaces
 * that are not needed.
 *
 * @param str The string to be modified.
 */
void trim_extra_spaces(char str[]);

/**
 * @brief Checks if a character is a space or a tab.
 *
 * This function determines whether a given character is either a space (' ') or a tab ('\t').
 *
 * @param c The character to check.
 * @return Returns 1 if the character is a space or tab, otherwise returns 0.
 */
int is_space_or_tab(char c);

/**
 * @brief Removes spaces surrounding a comma in a string.
 *
 * This function modifies a string by removing any white spaces adjacent to commas.
 *
 * @param str The string to be modified.
 */
void trim_spaces_around_comma(char *str);
