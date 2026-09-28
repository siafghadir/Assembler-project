#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "table.h"
#include "helper_functions.h"
#include "Errors.h"

/* This array defines error codes and their corresponding messages.
 * Some entries are intentionally left blank to accommodate future error codes.
 */
Error errors[] = {
        {ERROR_CODE_0,  "No Error"},
        {ERROR_CODE_1,  "Memory allocation failed"},
        {ERROR_CODE_2,  "Missing .as file name"},
        {ERROR_CODE_3,  "File name exceeds the maximum allowed length"},
        {ERROR_CODE_4,  "The specified file does not exist"},
        {ERROR_CODE_5,  "Missing .am file"},
        {ERROR_CODE_6,  "Line exceeds the maximum length"},
        {ERROR_CODE_7,  "Failed to open a new file for writing"},
        {ERROR_CODE_8,  "Failed to open the file for reading"},
        {ERROR_CODE_9,  "Macro definition missing a name"},
        {ERROR_CODE_10, "Extra text found after macro name definition"},
        {ERROR_CODE_11, "File position setting failed"},
        {ERROR_CODE_12, "endmcro statement followed by extra text"},
        {ERROR_CODE_13, "Macro is defined multiple times"},
        {ERROR_CODE_14, "File copy failed during macro expansion"},
        {ERROR_CODE_15, "Macro expansion failed in the .as file"},
        {ERROR_CODE_16, "Macro call before its declaration"},
        {ERROR_CODE_17, "Invalid macro name"},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {ERROR_CODE_30, "Line exceeds maximum allowed length"},
        {ERROR_CODE_31, "Invalid opcode"},
        {ERROR_CODE_32, "Line contains additional, unrecognized text"},
        {ERROR_CODE_33, "Invalid argument"},
        {ERROR_CODE_34, "Argument missing"},
        {ERROR_CODE_35, "Missing comma between arguments in a command line with two arguments"},
        {ERROR_CODE_36, "Label not defined in the assembly file"},
        {ERROR_CODE_37, "Invalid label following .entry directive"},
        {ERROR_CODE_38, "Invalid comma placement near opcode"},
        {ERROR_CODE_39, "Too many commas"},
        {ERROR_CODE_40, "Comma in an incorrect position"},
        {ERROR_CODE_41, "Invalid character near opcode or label"},
        {ERROR_CODE_42, "Invalid character detected"},
        {ERROR_CODE_43, "Missing comma between numbers"},
        {ERROR_CODE_44, "Invalid label declaration"},
        {ERROR_CODE_45, "Missing ':' after label declaration"},
        {ERROR_CODE_46, "Invalid register name. Use only r1-r7"},
        {ERROR_CODE_47, "Invalid comma before opcode"},
        {ERROR_CODE_48, ""},
        {ERROR_CODE_49, "Data line missing '.' before directive"},
        {ERROR_CODE_50, "Instruction '.data' line contains non-numeric data"},
        {ERROR_CODE_51, "Comma found after the last number in a '.data' line"},
        {ERROR_CODE_52, "Missing '\"' after .string directive"},
        {ERROR_CODE_53, "Extra text detected after the string in a '.string' line"},
        {ERROR_CODE_54, "Instruction Counter (IC) exceeds maximum CPU word length"},
        {ERROR_CODE_55, "Label defined multiple times"},
        {ERROR_CODE_56, "Label defined as .extern and also within the file"},
        {ERROR_CODE_57, "Number in .data line is out of range"},
        {ERROR_CODE_58, "Invalid data line directive"},
        {ERROR_CODE_59, "Instruction '.data' line contains invalid characters or syntax error"}
};


void display_internal_error(int error_code) {
    /* Output the error code and its corresponding message */
    printf("~~ERROR: ID:%d~~ | %s\n", error_code, errors[error_code].error_msg);
}


void display_external_error(int error_code, location file) {
    /* Output the error code, file name, line number, and corresponding error message */
    printf("~~ERROR: ID:%d~~ in %s at line:%d | Error: %s\n", error_code, 
           file.file_name, file.line_num, errors[error_code].error_msg);
}

