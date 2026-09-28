#ifndef LABRATORY_C_FINAL_PROJECT_ERRORS_H
#define LABRATORY_C_FINAL_PROJECT_ERRORS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "project_constants.h"
#include "table.h"
#include "helper_functions.h"

/* Structure representing an error with a unique ID and associated message */
typedef struct Error {
    int error_id;     /* Unique ID for identifying the error */
    char *error_msg;  /* Message describing the error */
} Error;

/* Enumeration defining error codes for various error types */
typedef enum ERROR_CODES {
    /* General and macro-related errors */
    ERROR_CODE_0 = 0,
    ERROR_CODE_1,
    ERROR_CODE_2,
    ERROR_CODE_3,
    ERROR_CODE_4,
    ERROR_CODE_5,
    ERROR_CODE_6,
    ERROR_CODE_7,
    ERROR_CODE_8,
    ERROR_CODE_9,
    ERROR_CODE_10,
    ERROR_CODE_11,
    ERROR_CODE_12,
    ERROR_CODE_13,
    ERROR_CODE_14,
    ERROR_CODE_15,
    ERROR_CODE_16,
    ERROR_CODE_17,
    /* Reserved space for future macro-related errors */

    /* Errors related to processing command assembly lines */
    ERROR_CODE_30 = 30,
    ERROR_CODE_31,
    ERROR_CODE_32,
    ERROR_CODE_33,
    ERROR_CODE_34,
    ERROR_CODE_35,
    ERROR_CODE_36,
    ERROR_CODE_37,
    ERROR_CODE_38,
    ERROR_CODE_39,
    ERROR_CODE_40,
    ERROR_CODE_41,
    ERROR_CODE_42,
    ERROR_CODE_43,
    ERROR_CODE_44,
    ERROR_CODE_45,
    ERROR_CODE_46,
    ERROR_CODE_47,
    ERROR_CODE_48,
    ERROR_CODE_49,
    /* Reserved space for future command processing errors */

    /* Errors related to reading .data or .string instruction lines */
    ERROR_CODE_50 = 50,
    ERROR_CODE_51,
    ERROR_CODE_52,
    ERROR_CODE_53,
    ERROR_CODE_54,
    ERROR_CODE_55,
    ERROR_CODE_56,
    ERROR_CODE_57,
    ERROR_CODE_58,
    ERROR_CODE_59
} ERROR_CODES;

/**
 * @brief Displays an external error message.
 *
 * This function outputs an error message related to the source file,
 * including the error code and the location within the source file.
 * 
 * @param error_code The error code indicating the type of error.
 * @param file A pointer to a structure containing the source file name and line details.
 */
void display_external_error(int error_code, location file);

/**
 * @brief Displays an internal error message.
 *
 * This function outputs an error message that originates from internal processes
 * and not from the source file itself.
 * 
 * @param error_code The error code indicating the type of internal error.
 */
void display_internal_error(int error_code);

#endif
