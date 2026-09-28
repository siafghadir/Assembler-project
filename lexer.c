#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "project_constants.h"
#include "Errors.h"

/* Define the opcodes */
op_code OPCODES[] = {
        {"mov",  2},
        {"cmp",  2},
        {"add",  2},
        {"sub",  2},
        {"lea",  2},
        {"clr",  1},
        {"not",  2},
        {"inc",  1},
        {"dec",  1},
        {"jmp",  1},
        {"bne",  1},
        {"red",  1},
        {"prn",  1},
        {"jsr",  1},
        {"rts",  0},
        {"stop", 0}
};
/* Define the registers */

char *REGS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Define the instructions */
char *INSTRUCTIONS[] = {".data", ".string", ".extern", ".entry"};


/* Function to count occurrences of a character in a string */
int count_character_occurrences(char *str, char ch) {
    int count = 0;
    char *ptr = str;

    while ((*ptr != '\0') && (ptr = strchr(ptr, ch)) != NULL) {
        count++;
        ptr++;
    }

    return count;
}

/* Function to check for whitespace in a string */
int contains_whitespace(const char *str) {
    while (*str) {
        if (isspace(*str)) {
            return 1;
        }
        str++;
    }

    return 0;
}

/* Function to check if a string is a legal sequence of digits, commas, and spaces */
int validate_string(const char *str) {
    int has_digit = 0, has_comma = 0;
    int i;

    for (i = 0; str[i]; i++) {
        if (isdigit(str[i]) || str[i] == '-' || str[i] == ' ') {
            if (has_comma) has_comma = 0;
            if (isdigit(str[i])) has_digit = 1;
        } else if (str[i] == ',') {
            if (!has_digit || has_comma) return 0;
            has_comma = 1;
        } else {
            return 0;
        }
    }

    return !has_comma;
}

/* Function to check if a string is a valid number */
int is_valid_number(char *str) {
    char *ptr;

    if (str) {
        strtol(str, &ptr, 10);
        if (*ptr == '\0' || *ptr == ' ') {
            return 1;
        }
    }

    return 0;
}

/* Function to check if a string is a valid number with a '#' prefix */
int is_number_with_prefix(char *str) {
    char *ptr;

    if (str && str[0] == '#') {
        strtol(str + 1, &ptr, 10);
        if (*ptr == '\0' || *ptr == ' ') {
            return 1;
        }
    }

    return 0;
}

/* Function to check for extra text in a string */
int has_extra_text() {
    char *token = strtok(NULL, "\n");
    return token != NULL;
}

/*end of utility functions*/

/* Function to identify a register from a string */
int identify_register(char *str) {
    int i;
    if (!str) return -1;

    for (i = 0; i < REG_COUNT; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i;
        }
    }

    return -1;
}

/* Function to determine the opcode from a string */
int identify_opcode(char *str) {
    int i;
    if (!str) return -1;

    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES[i].opcode) == 0) {
            return i;
        }
    }

    return -1;
}
/* Function to identify a pointer from a string */
int identify_pointer(char *operand) {
    if (!operand) return -1;

    if (*operand == '*') {
        int register_number = identify_register(operand + 1);
        if (register_number >= 0) return register_number;
    }

    return -1;
}
/* Function to check if a pointer is valid */
int is_valid_pointer(char *operand) {
    if (!operand) return 0;

    if (*operand == '*') {
        int register_number = identify_register(operand + 1);
        if (register_number >= 0) return 1;
    }

    return 0;
}
/* Function to validate a label */
int validate_label(char *str) {
    if (!str) return 0;

    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && identify_opcode(str) < 0 && identify_register(str) < 0 && !is_instruction(str)) {
        while (*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str)));
    }

    if (*str == '\0' || *str == ' ') {
        return 1;
    }

    return 0;
}


/* Function to check if a string is a valid instruction */
int is_instruction(char *str) {
    int i;
    if (!str) return 0;

    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTRUCTIONS[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

/*end of basic identific*/


/* Function to check if a label declaration is legal */
int validate_label_declaration(char *str, int *error_code) {
    if (!str) return 0;

    if (strlen(str) > MAX_LABEL_LENGTH || !isalpha(*str) || identify_register(str) >= 0 || is_valid_pointer(str)) {
        *error_code = ERROR_CODE_44;
        return 0;
    }

    if (identify_opcode(str) < 0) {
        while (*(++str) != '\0' && (isalpha(*str) || isdigit(*str)));
        if (*(str) == ':' && *(str + 1) == '\0') {
            *str = '\0';
            return 1;
        } else {
            *error_code = ERROR_CODE_44;
        }
    }

    return 0;
}


int check_line_length(char *file_name) {
    char buffer[BIG_NUMBER_CONST];
    FILE *file_ptr;
    location file_location;
    int line_is_too_long = 0;

    /* Open the specified file for reading */
    file_ptr = fopen(file_name, "r");
    file_location.file_name = file_name;

    /* Read through each line and verify its length */
    while (fgets(buffer, BIG_NUMBER_CONST, file_ptr) != NULL) {
        file_location.line_num++;
        if (strlen(buffer) > MAX_LINE_LENGTH) {
            display_external_error(ERROR_CODE_30, file_location);
            line_is_too_long = 1;
        }
    }

    fclose(file_ptr);
    return line_is_too_long;
}


/* Function to check if a string is a valid register or label */
int is_register_or_label(char *str) {
    return (identify_register(str) >= 0) || validate_label(str);
}

/* Function to check if a string is a valid register, label, or number */
int is_register_or_label_or_number(char *str) {
    return is_register_or_label(str) || is_number_with_prefix(str);
}

/* Function to validate the first argument in a string */
int validate_first_argument(char *str, char *ptr) {
    int first_arg_len = (int)(ptr - str);
    char first_arg[MAX_LINE_LENGTH];
    strncpy(first_arg, str, first_arg_len);
    first_arg[first_arg_len] = '\0';

    return is_register_or_label_or_number(first_arg);
}

/* Function to validate an argument based on the command */
int validate_argument(char *str, command_parts *command, int *error_code) {
    char *str1, *str2, *ptr;

    if (!str && OPCODES[command->opcode].arg_num != 0) {
        *error_code = ERROR_CODE_34;
        return 0;
    }

    if (OPCODES[command->opcode].arg_num == 0) {
        if (has_extra_text()) {
            *error_code = ERROR_CODE_32;
            return 0;
        }
        command->source = command->dest = NULL;
        return 1;
    }

    if (OPCODES[command->opcode].arg_num == 2) {
        if (!strstr(str, ",")) {
            *error_code = ERROR_CODE_35;
            return 0;
        } else if (count_character_occurrences(str, ',') > 1) {
            *error_code = ERROR_CODE_39;
            return 0;
        }

        str1 = strtok(str, ",");
        if ((ptr = strchr(str1, ' '))) {
            if (validate_first_argument(str1, ptr)) {
                *error_code = ERROR_CODE_35;
                return 0;
            }
            *error_code = ERROR_CODE_33;
            return 0;
        }

        str2 = strtok(NULL, " \n");
        if (has_extra_text()) {
            *error_code = ERROR_CODE_32;
            return 0;
        }
    } else if (OPCODES[command->opcode].arg_num == 1) {
        if (strchr(str, ' ')) {
            *error_code = ERROR_CODE_32;
            return 0;
        }
    }

    switch (command->opcode) {
        case 1:
            if ((is_register_or_label_or_number(str1) || is_valid_pointer(str1)) && 
                (is_register_or_label_or_number(str2) || is_valid_pointer(str2))) {
                command->source = str1;
                command->dest = str2;
            } else {
                if (!str2) {
                    *error_code = ERROR_CODE_34;
                } else if (identify_register(str1) == -1 || identify_register(str2) == -1) {
                    *error_code = ERROR_CODE_46;
                } else {
                    *error_code = ERROR_CODE_33;
                }
                return 0;
            }
            break;

        case 0:
        case 2:
        case 3:
            if ((is_register_or_label_or_number(str1) || is_valid_pointer(str1)) && 
                (is_register_or_label(str2) || is_valid_pointer(str2))) {
                command->source = str1;
                command->dest = str2;
            } else {
                validate_register(error_code, str1);
                validate_register(error_code, str2);
                return 0;
            }
            break;

        case 4:
            if (validate_label(str1) && (is_register_or_label(str2) || is_valid_pointer(str2))) {
                command->source = str1;
                command->dest = str2;
            } else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;

        case 5:
        case 6:
        case 7:
        case 8:
        case 11:
            if (is_register_or_label(str) || is_valid_pointer(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                if (atoi(strtok(str, "r"))) {
                    *error_code = ERROR_CODE_46;
                } else {
                    *error_code = ERROR_CODE_33;
                }
                return 0;
            }
            break;

        case 9:
        case 10:
        case 13:
            if (validate_label(str) || is_valid_pointer(str)) {
                command->source = NULL;
                command->dest = str;
            }
            break;

        case 12:
            if (is_register_or_label_or_number(str) || is_valid_pointer(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                validate_register(error_code, str);
                return 0;
            }
            break;

        default:
            *error_code = ERROR_CODE_33;
            return 0;
    }

    return 1;
}

/* Function to check for a comma after a directive */
int check_comma_after_directive(char *str, int *error_code) {
    if (strchr(str, ',')) {
        *error_code = ERROR_CODE_40;
        return 1;
    }
    *error_code = ERROR_CODE_58;
    return 0;
}

/* Function to check for register-related errors */
void validate_register(int *error_code, char *str) {
    char temp_str[MAX_LINE_LENGTH];
    strcpy(temp_str, str);

    if (*error_code) return;

    if (contains_whitespace(str)) {
        *error_code = ERROR_CODE_33;
    } else if (atoi(strtok(temp_str, "r"))) {
        *error_code = ERROR_CODE_46;
    } else {
        *error_code = ERROR_CODE_33;
    }
}

/* Function to parse a command */
command_parts *parse_command(char *str, int *error_code) {
    char *token;
    int visited_flag = 0;

    command_parts *command = handle_malloc(sizeof(command_parts));
    if (!command) return command;

    if (!add_space_after_colon(&str, error_code)) return command;

    token = strtok(str, " \n");

    if (validate_label_declaration(token, error_code)) {
        visited_flag = 1;
        command->label = token;

        token = strtok(NULL, " \n");
        if ((command->opcode = identify_opcode(token)) != -1) {;}
        else {
            *error_code = check_opcode_errors(token);
            command->opcode = -1;
            return command;
        }

        if (OPCODES[command->opcode].arg_num == 0) {
            if (has_extra_text()) {
                *error_code = ERROR_CODE_32;
            } else {
                command->source = command->dest = NULL;
            }
        } else {
            if (!validate_argument(strtok(NULL, "\n"), command, error_code)) {
                return command;
            }
        }
    } else {
        if (*error_code) return command;
    }

    if (!visited_flag) {
        if ((command->opcode = identify_opcode(token)) != -1) {
            command->label = NULL;
            validate_argument(strtok(NULL, "\n"), command, error_code);
            return command;
        } else {
            *error_code = check_opcode_errors(token);
            command->opcode = -1;
            return command;
        }
    }

    return command;
}

/* Function to parse entry or extern instructions */
inst_parts *parse_entry_or_extern(char *str, int *error_code) {
    inst_parts *inst;
    char *ptr, *token;

    ptr = strchr(str, '.');
    token = strtok(ptr, " ");

    inst = handle_malloc(sizeof(inst_parts));
    if (!inst) {
        *error_code = ERROR_CODE_1;
        return NULL;
    }

    inst->label = NULL;
    inst->nums = NULL;
    inst->is_extern = 0;
    inst->len = 0;

    if (strcmp(token, ".extern") == 0) {
        inst->is_extern = 1;
    }

    token = strtok(NULL, " \n");
    if (validate_label(token)) {
        inst->arg_label = token;
    } else {
        *error_code = ERROR_CODE_44;
    }

    if (has_extra_text()) {
        *error_code = ERROR_CODE_32;
    }

    return inst;
}

/* Function to parse an instruction */
inst_parts *parse_instruction(char *str, int *error_code) {
    inst_parts *inst;
    char *token;
    char token_copy[MAX_LINE_LENGTH];
    strcpy(token_copy, str);

    if (!strstr(str, ".")) return 0;

    if (!add_space_after_colon(&str, error_code)) return NULL;

    token = strtok(str, " \n");

    inst = handle_malloc(sizeof(inst_parts));
    if (!inst) {
        *error_code = ERROR_CODE_1;
        return NULL;
    }

    inst->label = NULL;
    inst->nums = NULL;

    if (validate_label_declaration(token, error_code)) {
        inst->label = token;
        token = strtok(NULL, " \n");
    } else if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {
        inst->label = NULL;
    }

    if (strcmp(token, ".data") == 0) {
        extract_numbers(str, token_copy, inst, error_code);
    } else if (strcmp(token, ".string") == 0) {
        process_string_instruction(str, inst, error_code);
    } else if (strcmp(token, ".entry") == 0) {
        token = strtok(NULL, " \n");
        if (validate_label(token)) {
            inst->label = NULL;
            inst->len = -1;
            inst->arg_label = token;
            inst->nums = 0;
            inst->is_extern = 0;
        } else {
            *error_code = ERROR_CODE_37;
            free(inst);
            return 0;
        }
    } else if (strcmp(token, ".extern") == 0) {
        token = strtok(NULL, " \n");
        if (validate_label(token)) {
            inst->label = NULL;
            inst->len = -1;
            inst->arg_label = token;
            inst->nums = 0;
            inst->is_extern = 1;
        }
    } else {
        check_comma_after_directive(token, error_code);
    }

    return inst;
}

/* Function to extract numbers from a string */
int extract_numbers(char *str, char *token_copy, inst_parts *inst, int *error_code) {
    char *token;
    int len = 0, number;

    if (!add_space_after_colon(&token_copy, error_code)) return 0;

    token = strtok(NULL, " \n");
    if (!validate_string(token)) {
        *error_code = ERROR_CODE_59;
        return 0;
    }

    strtok(token_copy, " \n");
    strtok(NULL, " \n");

    while ((token = strtok(NULL, ", \n")) != NULL) {
        if (is_valid_number(token)) {
            number = (short)(atoi(token));
            if (number > MAX_NUM || number < MIN_NUM) {
                *error_code = ERROR_CODE_57;
                return 0;
            } else if (!increase_array(&inst, ++len)) {
                return 0;
            }
            *(inst->nums + len - 1) = (short)(atoi(token));
        } else {
            if (strcmp(token, "\n") == 0) {
                *error_code = ERROR_CODE_51;
                return 0;
            }
            *error_code = ERROR_CODE_50;
            return 0;
        }
    }

    inst->len = len;
    return 1;
}

/* Function to process a string instruction */
int process_string_instruction(char *str, inst_parts *inst, int *error_code) {
    int char_saved_flag = 0;
    char *token;
    int len = inst->len = 0;

    if (*(token = strtok(NULL, "\n")) != '"') {
        *error_code = ERROR_CODE_52;
        return 0;
    }
    token++;

    if (!strchr(token, '"')) {
        *error_code = ERROR_CODE_52;
        return 0;
    }

    while (*(token + len) != '"') {
        if (!increase_array(&inst, ++len)) return 0;
        *(inst->nums + len - 1) = (short)(*(token + len - 1));
        char_saved_flag = 1;
    }

    if (!(*(token + len + 1) == '\0' || *(token + len + 1) == '\n')) {
        *error_code = ERROR_CODE_53;
        if (char_saved_flag) {
            free(inst->nums);
        }
        return 0;
    }
    if (!increase_array(&inst, ++len)) return 0;
    *(inst->nums + len - 1) = 0;
    inst->len = len;

    return 1;
}

/* Function to dynamically increase an array */
int increase_array(inst_parts **inst, int len) {
    short *backup = (*inst)->nums;

    (*inst)->nums = realloc((*inst)->nums, (len + 1) * sizeof(short));
    if (!(*inst)->nums) {
        display_internal_error(ERROR_CODE_1);
        free(backup);
        return 0;
    }

    return 1;
}

/* Function to check for opcode-related errors */
int check_opcode_errors(char *str) {
    char *comma_ptr = strchr(str, ',');

    return comma_ptr ? ERROR_CODE_38 : ERROR_CODE_31;
}

/* Function to add a space after a colon in a string */
int add_space_after_colon(char **str, int *error_code) {
    char *colon_ptr = strchr(*str, ':');

    if (!colon_ptr) return 1;

    if (strlen(*str) == MAX_LINE_LENGTH) {
        char *temp_ptr = *str;

        *str = realloc(*str, MAX_LINE_LENGTH + 1);
        if (!*str) {
            *error_code = ERROR_CODE_1;
            free(temp_ptr);
            return 0;
        }
    }

    colon_ptr = strchr(*str, ':');
    colon_ptr++;
    memmove(colon_ptr + 1, colon_ptr, strlen(colon_ptr) + 1);
    *colon_ptr = ' ';

    return 1;
}


