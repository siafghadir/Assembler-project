#ifndef LABRATORY_C_FINAL_PROJECT_HELPER_FUNCTIONS_H
#define LABRATORY_C_FINAL_PROJECT_HELPER_FUNCTIONS_H

#include "project_constants.h"

/* Memory Management Function */
/**
 * @brief Allocates memory and handles any potential errors that occur.
 *
 * This function allocates memory of the specified size and checks for allocation errors.
 * If memory allocation fails, it returns NULL.
 *
 * @param object_size The size of the memory to allocate.
 * @return A void pointer to the allocated memory, or NULL if the allocation fails.
 */
void *handle_malloc(long object_size);

/* Conversion Functions */
/**
 * @brief Converts a negative short number to an unsigned short using two's complement.
 *
 * This function converts a negative short integer into its unsigned short representation
 * using the two's complement method.
 *
 * @param pos_num A negative short number to be converted.
 * @return The unsigned short representation of the number using two's complement.
 */
unsigned short twos_compliment(unsigned short pos_num);

/**
 * @brief Converts an unsigned short integer to its binary string representation.
 *
 * This function converts an unsigned short integer into a binary string.
 * The caller is responsible for freeing the memory allocated for the string.
 *
 * @param num The unsigned short integer to convert.
 * @return A dynamically allocated string containing the binary representation of the given number.
 */
char *convert_short_to_binary(unsigned short num);

/**
 * @brief Converts an unsigned short number to its octal string representation.
 *
 * This function takes an unsigned short integer and converts it into a string
 * representing its octal value.
 *
 * @param num The unsigned short number to convert.
 * @return A string containing the octal representation of the given number.
 */
char *convert_short_to_octal(unsigned short num);

/* File Management Functions */
/**
 * @brief Creates a new file name by appending a new extension.
 *
 * This function generates a new file name by removing the original extension, if present,
 * and appending a new specified extension.
 *
 * @param file_name The base string of the original file name.
 * @param ending The string representing the new file extension.
 * @return A string containing the new file name with the specified extension.
 */
char *add_new_file(char *file_name, char *ending);

/**
 * @brief Copies the contents of one file to another.
 *
 * This function creates an exact copy of a file by copying the contents from the original file
 * to a new file specified by the destination file name.
 *
 * @param file_name_dest The name of the destination file.
 * @param file_name_orig The name of the original file to be copied.
 * @return Returns 1 if the file was copied successfully, otherwise returns 0.
 */
int copy_file(char *file_name_dest, char *file_name_orig);

/**
 * @brief Manages the proper closure of opened files and deallocation of file name strings.
 *
 * This function ensures that all files are properly closed and associated strings are deallocated.
 * It handles a variable number of arguments, where each file or string identifier is paired with the file or string itself.
 *
 * @param num_args The number of arguments, representing twice the number of files and strings to handle.
 * @param ... A list of pairs consisting of a file or string identifier ("%s" for string or "file" for file) and the actual file or string.
 */
void abrupt_close(int num_args, ...);

#endif
