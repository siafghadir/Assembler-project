#ifndef FIRST_EXE_H
#define FIRST_EXE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "project_constants.h"
#include "table.h"
#include "helper_functions.h"
#include "first_exe.h"
#include "lexer.h"
#include "encoding.h"
#include "second_exe.h"

/**
 * @brief Performs the first pass of assembly file processing.
 *
 * This function parses and processes the assembly file during the first pass.
 *
 * @param file_name The name of the assembly file to be processed.
 * @return An integer representing the success or failure of the first pass.
 *         - Returns 0 if successful.
 *         - Returns a non-zero value if an error is encountered.
 */
int exe_first_pass(char *file_name);

/**
 * @brief Manages memory allocation for structures used during the first pass.
 *
 * This function allocates memory for the various tables and structures used
 * in the first pass of assembly processing.
 *
 * @param externs Pointer to the pointer of the externs table.
 * @param entries Pointer to the pointer of the entries table.
 * @param code Pointer to the pointer of the code_conv structure for machine code.
 * @param data Pointer to the pointer of the code_conv structure for data.
 * @return An integer indicating the success or failure of the allocation.
 *         - Returns 0 if successful.
 *         - Returns a non-zero value if an error occurs during allocation.
 */
int handle_allocation(other_table **externs, other_table **entries, code_conv **code, code_conv **data);

#endif
