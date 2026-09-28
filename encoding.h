#ifndef C_PROJECT_ENCODING_H
#define C_PROJECT_ENCODING_H

#include "lexer.h"
#include "project_constants.h"

#define SHIFT_TO_SOURCE 7    /* Shift left by 7 bits to align the source operand's LSB (7-10) */
#define SHIFT_TO_OPCODE 11   /* Shift left by 11 bits to align the opcode's LSB (11-14) */
#define SHIFT_TO_DEST 3      /* Shift left by 3 bits to align the destination operand's LSB (3-6) */

#define SHIFT_TO_SOURCE_REG 6 /* Shift left by 6 bits to align the source register's LSB (6-8) */
#define DEST_BITS_SHIFT_REG 3 /* Shift left by 3 bits to align the destination register's LSB (3-5) */

/* Addressing mode identifiers */
#define ADDRESSING_IS_DIRECT 1
#define ADDRESSING_IS_POINTER 4
#define ADDRESSING_IS_LABEL 2
#define ADDRESSING_IS_REG 8

/* Value used to mark both registers as already processed to prevent duplicate handling */
#define DOUBLE_REGS_VALUE 10000

/* Maximum value for the instruction counter */
#define IC_MAX 4095

/**
 * @brief Structure representing a translated assembly line.
 *
 * This structure holds the numerical representation of a converted assembly line,
 * an optional label associated with the line, and the corresponding line number in the source code.
 */
typedef struct code_conv {
    unsigned short short_num; /* Numeric representation of the converted line */
    char *label;              /* Optional label associated with the line */
    int assembly_line;        /* Line number in the assembly source code */
} code_conv;

/**
 * @brief Expands the memory allocated for an array of code_conv structures.
 *
 * This function reallocates memory for an array of code_conv structures, increasing its size based on the instruction counter (IC).
 * If successful, it returns 1. If reallocation fails, it frees the original memory, prints an error, and returns 0.
 *
 * @param code Pointer to the array of code_conv structures to be expanded.
 * @param IC New size for the array.
 * @return 1 if memory expansion succeeds, 0 otherwise.
 */
int expand_code_memory(code_conv **code, int IC);

/**
 * @brief Frees memory allocated for the code_conv array.
 *
 * This function releases the memory allocated to the code_conv array, including the labels associated with each line.
 * It iterates over the array, freeing the memory for each element, and then frees the array itself.
 *
 * @param code Pointer to the code_conv array.
 * @param code_count Number of elements in the code_conv array.
 */
void free_code(code_conv *code, int code_count);

/**
 * @brief Converts a command_parts structure into a 15-bit machine code representation.
 *
 * This function takes a command_parts structure and converts its source and destination operands, along with the opcode,
 * into a bitfield representation based on the addressing modes. The result is returned as an unsigned short.
 *
 * @param command Pointer to the command_parts structure containing the command information.
 * @return A 15-bit unsigned short representing the machine code.
 */
unsigned short convert_command_to_short(command_parts *command);

/**
 * @brief Converts register operands into bitfield representation.
 *
 * This function converts register operands from the command_parts structure into their corresponding bit representation.
 * If reg_src is true, both source and destination registers are converted; otherwise, only the destination register is processed.
 * The function ensures that double-register instructions are handled correctly.
 *
 * @param command Pointer to the command_parts structure containing command information.
 * @param reg_src Indicator if both source and destination registers should be converted.
 * @return A 15-bit unsigned short representing the register operand(s).
 */
unsigned short convert_reg_or_pointer_to_short(command_parts *command, int reg_src);

/**
 * @brief Adds a machine code line to the code_conv array.
 *
 * This function inserts a machine code value into the code_conv array at the specified instruction counter (IC) position.
 * It also associates an optional label with the line and increments the IC.
 *
 * @param code Pointer to the array of code_conv structures.
 * @param num Machine code value to be inserted.
 * @param str Optional label for the machine code line (can be NULL).
 * @param IC Pointer to the instruction counter (IC).
 * @param am_file Structure representing the file's location in memory.
 * @return 1 if the insertion is successful, 0 otherwise.
 */
int insert_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file);

/**
 * @brief Converts additional command line components to machine code and stores them.
 *
 * This function handles the conversion of optional second and third operands in a command line to machine code.
 * It stores the resulting machine words in the code_conv array.
 *
 * @param code Pointer to the array of code_conv structures.
 * @param command Pointer to the command_parts structure containing the parsed command line.
 * @param IC Pointer to the instruction counter (IC).
 * @param is_src Indicator if the conversion is for the source (1) or destination (0).
 * @param am_file Structure representing the file's location in memory.
 * @return 1 if the operation is successful, 0 otherwise.
 */
int insert_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file);

/**
 * @brief Inserts machine code data into the data array.
 *
 * This function adds machine code values from the inst_parts structure to a data array, updating the data counter (DC).
 * It iterates through the data values and inserts each into the array, incrementing the DC accordingly.
 *
 * @param data Pointer to the array of code_conv structures where data will be stored.
 * @param inst Structure containing the data values to be added.
 * @param DC Pointer to the data counter (DC).
 * @param am_file Structure representing the file's location in memory.
 * @return 1 if the operation is successful, 0 otherwise.
 */
int insert_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file);

/**
 * @brief Merges code and data arrays into a unified code array.
 *
 * This function combines the machine code array and the data array into a single contiguous array.
 * It extends the code array to accommodate the data array and copies the data elements starting from the current IC.
 *
 * @param code Pointer to the array of code_conv structures containing machine code.
 * @param data Pointer to the array of code_conv structures containing data.
 * @param IC Current instruction counter (IC).
 * @param DC Data counter (DC).
 * @return 1 if the merge is successful, 0 otherwise.
 */
int merge_machine_code_and_data(code_conv **code, code_conv *data, int IC, int DC);

/**
 * @brief Outputs the machine code in binary format to stdout.
 *
 * This function prints the binary representation of each machine word in the code_conv array.
 *
 * @param code Pointer to the code_conv array containing the machine words.
 * @param IC_len Number of lines in the code_conv array.
 */
void print_binary_code(code_conv *code, int IC_len);

#endif
