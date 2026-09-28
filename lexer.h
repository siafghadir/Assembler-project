#ifndef C_PROJECT_LEXER_H
#define C_PROJECT_LEXER_H

/* Structure to define an operation code, including its associated opcode and the number of arguments */
typedef struct op_code {
    char *opcode;    /* String representing the operation's opcode */
    int arg_num;     /* Number of arguments required by the operation */
} op_code;

/* Structure to define the components of a command */
typedef struct command_parts {
    char *label;     /* The label tied to the command */
    int opcode;      /* Opcode value of the command */
    char *source;    /* Source operand for the command */
    char *dest;      /* Destination operand for the command */
} command_parts;

/* Structure to define the components of a data or instruction line */
typedef struct inst_parts {
    char *label;       /* The label associated with the instruction */
    short *nums;       /* Array of short integers representing data */
    int len;           /* Count of elements in *nums, including the trailing '0' */
    char *arg_label;   /* Optional label associated with an argument */
    int is_extern;     /* Flag to indicate if the instruction is external */
} inst_parts;


/**
 * @brief Parses a string containing an entry or extern directive.
 *
 * The function processes an input string to extract details of an entry or extern directive,
 * populating an inst_parts structure with this information. It handles memory allocation for the structure
 * and returns a pointer to it. In case of errors, it updates the provided error code.
 *
 * @param str: The input string with the directive.
 * @param error_code: Pointer to an integer for storing error codes.
 * @return Returns a pointer to the inst_parts structure containing the parsed details, or NULL on error.
 */
inst_parts *parse_entry_or_extern(char *str, int *error_code);


/**
 * @brief Verifies if any line in the specified file exceeds the maximum allowed length.
 *
 * The function reads through each line of the file to check if its length surpasses `MAX_LINE_LENGTH`.
 * If a long line is found, it triggers an error using `ERROR_CODE_30` and sets a flag.
 *
 * @param file_name A pointer to a string representing the file name.
 *
 * @return Returns 1 if any line exceeds `MAX_LINE_LENGTH`, otherwise returns 0.
 */
int check_line_length(char *file_name);


/**
 * @brief Determines if a given string corresponds to an instruction.
 *
 * The function compares the input string against known instructions. If a match is found,
 * it returns 1, indicating that the string is an instruction. If no match is found, it returns 0.
 *
 * @param str A pointer to a string representing the potential instruction.
 *
 * @return Returns 1 if the string is an instruction, otherwise returns 0.
 */
int is_instruction(char *str);


/**
 * @brief Returns the index of an opcode in a predefined list.
 *
 * This function searches for the given string within a predefined array of opcodes.
 * If found, it returns the corresponding index; otherwise, it returns -1.
 *
 * @param str A pointer to a string representing the opcode.
 *
 * @return Returns the index of the opcode in the predefined array, or -1 if not found.
 */
int identify_opcode(char *str);


/**
 * @brief Retrieves the index of a register from a predefined list.
 *
 * This function searches for the given string within a predefined array of registers.
 * If a match is found, it returns the corresponding index; otherwise, it returns -1.
 *
 * @param str A pointer to a string representing the register.
 *
 * @return Returns the index of the register in the predefined array, or -1 if not found.
 */
int identify_register(char *str);


/**
 * @brief Validates whether a string is a proper label declaration.
 *
 * The function checks if the input string follows the rules for label declaration:
 * - Must start with an alphabetic character
 * - Length should not exceed `MAX_LABEL_LENGTH`
 * - Must end with a colon ':' followed by a null character '\0'
 * - Should not match any predefined register or opcode
 *
 * If the string meets all criteria, it returns 1; otherwise, it returns 0.
 *
 * @param str A pointer to a string representing the label to validate.
 * @param error_code Pointer to an integer for storing error codes.
 *
 * @return Returns 1 if the string is a valid label declaration, otherwise returns 0.
 */
int validate_label_declaration(char *str, int *error_code);


/**
 * @brief Checks if a string is a valid label.
 *
 * This function ensures that the input string adheres to the label naming rules:
 * - The first character must be alphabetic
 * - Length should not exceed `MAX_LABEL_LENGTH`
 * - Must not be a register or an instruction
 * - Can contain alphanumeric characters after the first character
 *
 * If the string is a valid label, it returns 1; otherwise, it returns 0.
 *
 * @param str A pointer to a string representing the label.
 *
 * @return Returns 1 if the string is a valid label, otherwise returns 0.
 */
int validate_label(char *str);


/**
 * @brief Checks if extra text exists after a newline character in a token stream.
 *
 * The function uses `strtok` with a newline delimiter to determine if more text exists after the newline.
 * If extra text is found, it returns 1; otherwise, it returns 0.
 *
 * @return Returns 1 if extra text is present after a newline, otherwise returns 0.
 */
int has_extra_text();


/**
 * @brief Adds a space character after a colon (:) in the given string.
 *
 * The function locates the first colon in the input string and inserts a space after it.
 * If memory reallocation is needed and fails, it sets an error code.
 *
 * @param str Pointer to the input string. The string is modified in place.
 * @param error_code Pointer to an integer for storing error codes.
 *
 * @return Returns 1 if a space was successfully added, 0 if no colon was found or an error occurred.
 */
int add_space_after_colon(char **str, int *error_code);


/**
 * @brief Determines if a string is either a valid register or label.
 *
 * This function checks if the input string is a register using `identify_register` or a label using `validate_label`.
 * If either condition is met, it returns 1; otherwise, it returns 0.
 *
 * @param str A pointer to a string to validate.
 *
 * @return Returns 1 if the string is a register or label, otherwise returns 0.
 */
int is_register_or_label(char *str);


/**
 * @brief Verifies if a string contains errors related to register usage.
 *
 * The function checks if the input string contains whitespace and verifies if it is a valid register.
 * Depending on the findings, it sets the appropriate error code.
 *
 * @param error_code Pointer to an integer for storing error codes.
 * @param str The input string to check.
 */
void validate_register(int *error_code, char *str);


/**
 * @brief Determines if a string is a valid integer number.
 *
 * The function converts the input string to a long integer using `strtol`. If the entire string is a valid number,
 * it returns 1. If not, it returns 0.
 *
 * @param str A pointer to a string to check for numeric validity.
 *
 * @return Returns 1 if the string is a valid number, otherwise returns 0.
 */
int is_valid_number(char *str);

/**
 * @brief Checks if the provided string is a valid number with an # prefix.
 *
 * This function examines the input string to determine if it represents a valid number 
 * that may optionally start with a prefix (`#`). 
 *
 * @param str A pointer to a string that may represent a number with a prefix.
 * 
 * @return Returns 1 if the string is a valid number with an optional #; 
 *         otherwise, returns 0.
 */
int is_number_with_prefix(char *str);


/**
 * @brief Validates whether a string is a register, label, or number.
 *
 * The function checks if the string is a register or label using `is_register_or_label` and if it's a number using `is_valid_number`.
 * If it satisfies any of these conditions, it returns 1; otherwise, it returns 0.
 *
 * @param str A pointer to a string to validate.
 *
 * @return Returns 1 if the string is a register, label, or number, otherwise returns 0.
 */
int is_register_or_label_or_number(char *str);


/**
 * @brief Validates the legality of an argument for a specific command.
 *
 * The function checks if an argument is valid based on the command's opcode, setting an error code if necessary.
 * If valid, it assigns the argument to the correct field in the command structure.
 *
 * @param str A pointer to a string containing the argument.
 * @param command A pointer to a command_parts structure to which the argument belongs.
 * @param error_code Pointer to an integer for storing error codes.
 *
 * @return Returns 1 if the argument is valid and assigned correctly, otherwise returns 0.
 */
int validate_argument(char *str, command_parts *command, int *error_code);


/**
 * @brief Parses a command string and stores the result in a command_parts structure.
 *
 * The function tokenizes the input string, validates any labels and opcodes, and checks the arguments.
 * It allocates a command_parts structure and stores the parsed data.
 *
 * @param str The input string containing the command.
 * @param error_code Pointer to an integer for storing error codes.
 * @return Returns a pointer to the command_parts structure if parsing is successful, otherwise returns NULL.
 */
command_parts *parse_command(char *str, int *error_code);


/**
 * @brief Extracts numbers from a string and stores them in the nums array of an inst_parts structure.
 *
 * The function tokenizes the input string and validates each token as a number, adding valid numbers to the nums array.
 * It handles memory allocation and sets an error code if an invalid token is found.
 *
 * @param str The input string containing the numbers.
 * @param token_copy A copy of the token being processed.
 * @param inst A pointer to an inst_parts structure where numbers will be stored.
 * @param error_code Pointer to an integer for storing error codes.
 * @return Returns 1 if all tokens are valid numbers, otherwise returns 0.
 */
int extract_numbers(char *str, char *token_copy, inst_parts *inst, int *error_code);


/**
 * @brief Parses an instruction string and stores the result in an inst_parts structure.
 *
 * The function checks for valid instruction types, tokenizes the input string, and processes numbers or strings accordingly.
 * It allocates an inst_parts structure and validates the instruction, setting an error code if necessary.
 *
 * @param str The input string containing the instruction.
 * @param error_code Pointer to an integer for storing error codes.
 * @return Returns a pointer to the inst_parts structure if parsing is successful, otherwise returns 0.
 */
inst_parts *parse_instruction(char *str, int *error_code);


/**
 * @brief Expands the nums array in an inst_parts structure.
 *
 * The function reallocates memory to increase the size of the nums array in the inst_parts structure.
 * If memory reallocation fails, it handles the error and frees previously allocated memory.
 *
 * @param inst A double pointer to an inst_parts structure.
 * @param len The new length for the nums array.
 * @return Returns 1 if memory reallocation is successful, otherwise returns 0.
 */
int increase_array(inst_parts **inst, int len);


/**
 * @brief Checks for errors related to opcode parsing.
 *
 * The function looks for the presence of a comma in the opcode string, indicating an error,
 * and returns the appropriate error code based on its findings.
 *
 * @param str The input string containing the opcode.
 * @return Returns the corresponding error code based on the presence of a comma.
 */
int check_opcode_errors(char *str);


/**
 * @brief Counts the occurrences of a specified character in a string.
 *
 * The function iterates through the input string and counts how many times a specified character appears.
 *
 * @param str A pointer to a string in which to count the occurrences of the character.
 * @param ch The character to count within the string.
 * @return Returns the count of the specified character in the string.
 */
int count_character_occurrences(char *str, char ch);


/**
 * @brief Checks if a comma appears after a directive in a string.
 *
 * The function verifies if a comma follows a directive in the input string. If found, it sets an error code and returns 1.
 * If no comma is found but other extraneous characters are present, it returns 0 and sets a different error code.
 *
 * @param str The input string to check.
 * @param error_code Pointer to an integer for storing error codes.
 * @return Returns 1 if a comma follows a directive, otherwise returns 0 and sets an error code.
 */
int check_comma_after_directive(char *str, int *error_code);


/**
 * @brief Verifies the validity of a string representing a sequence of integers separated by commas.
 *
 * The function checks if the input string forms a valid sequence of integers, allowing negative numbers and spaces.
 * If the string is valid, it returns 1, otherwise, it returns 0.
 *
 * @param str The input string to validate.
 * @return Returns 1 if the string represents a valid sequence of integers, otherwise returns 0.
 */
int validate_string(const char *str);


/**
 * @brief Validates the first argument in a string.
 *
 * This function extracts and validates the first argument from a string, checking if it's a register, label, or number.
 *
 * @param str The input string containing the argument list.
 * @param ptr Pointer to the character in `str` where the first argument ends.
 * @return Returns 1 for a register, 2 for a label, 3 for a number, or 0 if invalid.
 */
int validate_first_argument(char *str, char *ptr);


/**
 * @brief Processes a string instruction and stores its characters as shorts in the nums array of an inst_parts structure.
 *
 * The function tokenizes the input string, ensuring that the characters are enclosed in quotes, and stores them in the nums array.
 * If there is text outside the quotes, it sets an error code and returns 0.
 *
 * @param str The input string containing the instruction.
 * @param inst A pointer to an inst_parts structure where the string's characters will be stored.
 * @param error_code Pointer to an integer for storing error codes.
 * @return Returns 1 if the string was successfully processed, otherwise returns 0.
 */
int process_string_instruction(char *str, inst_parts *inst, int *error_code);


/**
 * @brief Identifies whether the provided operand is a pointer.
 *
 * The function checks the given operand to determine if it is a pointer, returning the corresponding index or identifier if valid.
 *
 * @param operand A pointer to a string representing the operand.
 * @return Returns the index or identifier of the pointer if valid, otherwise returns -1.
 */
int identify_pointer(char *operand);

/**
 * @brief Validates whether the provided operand is a valid pointer.
 *
 * This function checks if the operand follows the expected format for a valid pointer.
 * If valid, it returns 1, otherwise, it returns 0.
 *
 * @param operand A pointer to a string representing the operand.
 * @return Returns 1 if the operand is a valid pointer, otherwise returns 0.
 */
int is_valid_pointer(char *operand);

#endif
