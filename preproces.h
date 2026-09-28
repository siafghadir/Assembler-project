#ifndef PREPROCES
#define PREPROCES

#include <stdio.h>
#include "deal_with_data.h"

/**
 * @brief Reads and captures the content of a macro starting from a specific position in the source file.
 *
 * @param fp A file pointer to the source file.
 * @param pos A pointer to the file position indicating where the macro content begins.
 * @param line_count A pointer to an integer tracking the current line number being read.
 * @return A string containing the macro's content.
 */
char *capture_macro_content(FILE *fp, fpos_t *pos, int *line_count);

/**
 * @brief Validates the name of a macro according to predefined rules.
 *
 * This function checks if a macro name adheres to specific rules and ensures its validity.
 *
 * @param str A string containing the current line read from the source file.
 * @param name A double pointer to a string where the macro name will be stored.
 * @param line_count An integer representing the line number being processed.
 * @param file_name A string with the name of the source file.
 * @return Returns 1 if the macro name is valid, otherwise returns 0.
 */
int validate_macro_declaration(char *str, char **name, int line_count, char *file_name);

/**
 * @brief Checks for macro calls made before their declarations in the source file.
 *
 * This function verifies if any macro is called before its declaration in the source file.
 *
 * @param file_name A string with the name of the source file.
 * @param head A pointer to the head of a linked list where the macros are stored.
 * @return Returns 1 if there are macro calls before their declarations, otherwise returns 0.
 */
int check_macro_call_before_declaration(char file_name[], node *head);

/**
 * @brief Scans the source file to store all macros within it.
 *
 * This function processes the source file and stores all detected macros in a linked list.
 *
 * @param file_name A string containing the name of the source file.
 * @param head A double pointer to the head of a linked list where the macros will be stored.
 * @return Returns 1 if all macros are successfully stored, otherwise returns 0.
 */
int process_macros(char *file_name, node **head);

/**
 * @brief Expands a macro within a line and saves the result.
 *
 * This function replaces a macro call within a line with its full content and returns the modified line.
 *
 * @param str A string containing the line where the macro call is present.
 * @param mcro A pointer to the node in the linked list where the macro is stored.
 * @return A string with the line including the expanded macro content.
 */
char *replace_macro_with_content(char *str, node *mcro);

/**
 * @brief Replaces all macro occurrences in the source file and creates a modified file.
 *
 * This function scans the source file for macro calls and replaces them with their respective definitions,
 * generating a new file with the expanded content.
 *
 * @param file_name A string containing the name of the source file.
 * @param head A pointer to the head of a linked list where macros are stored.
 * @return A string representing the name of the newly created file with expanded macros.
 */
char *replace_all_macros(char file_name[], node *head);

/**
 * @brief Removes macro definitions from the source file and creates a new file.
 *
 * This function eliminates all macro declarations from the source file, resulting in a new file without the macro definitions.
 *
 * @param file_name A string with the name of the source file.
 * @return A string representing the name of the newly created file after macro definitions are removed.
 */
char *remove_macro_declarations(char file_name[]);

/**
 * @brief Executes the entire macro expansion process on the source file.
 *
 * This function handles the macro expansion for the source file, which includes:
 * 1. Removing excess white spaces and saving the result in a temporary file.
 * 2. Scanning and saving all macros in a linked list.
 * 3. Verifying that no macro is called before its declaration.
 * 4. Removing macro declarations and saving the result in a new temporary file.
 * 5. Replacing all macro calls with their corresponding definitions and saving the final output in a new file.
 * 6. Cleaning up temporary files and freeing allocated memory.
 *
 * @param file_name The name of the source file for macro expansion.
 * @return Returns 1 if the macro expansion process is successful, otherwise returns 0.
 */
int execute_macro_processing(char file_name[]);

#endif
