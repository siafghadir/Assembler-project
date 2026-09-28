#ifndef LABRATORY_C_FINAL_PROJECT_PROJECT_CONSTANTS_H
#define LABRATORY_C_FINAL_PROJECT_PROJECT_CONSTANTS_H

/* This file contains all global constants used throughout the program */

/* Maximum allowed length for a label in the command line */
#define MAX_LABEL_LENGTH 31

/* Maximum allowed length for a single line of code */
#define MAX_LINE_LENGTH 81

/* Initial value for the Instruction Counter (IC) */
#define IC_INIT_VALUE 100

/* A large constant used for line length in various operations */
#define BIG_NUMBER_CONST 1000

/* Total number of opcodes available */
#define OPCODES_COUNT 16

/* Total number of registers */
#define REG_COUNT 8

/* Length of a word in bits */
#define WORD_LEN 15

/* Number of bits used for ARE (Addressing, Relocation, Error) */
#define ARE_BITS 3

/* Value indicating an external symbol */
#define EXTERNAL_VALUE 1

/* Number of instruction types */
#define INSTRUCTIONS_COUNT 4

/* Maximum numeric value allowed */
#define MAX_NUM ((1 << (WORD_LEN-1)) - 1)

/* Minimum numeric value allowed */
#define MIN_NUM (-(1 << (WORD_LEN-1)))

/* Structure to store file location information */
typedef struct location {
    char *file_name;  /* Name of the file */
    int line_num;     /* Line number within the file */
} location;

/* Structure representing a line of data */
typedef struct line_data {
    char *file_name;  /* Name of the file associated with the line */
    short number;     /* Line number, useful for error tracking */
    char *data;       /* The actual content of the line */
} line_data;

#endif
