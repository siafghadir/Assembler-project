#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "encoding.h"
#include "Errors.h"
#include "project_constants.h"
#include "table.h"
#include "helper_functions.h"
#include "lexer.h"

int expand_code_memory(code_conv **code_ptr, int current_size) {
    code_conv *old_ptr;
    old_ptr = *code_ptr;
    /* increasing memory of code for a new word */
    *code_ptr = realloc(*code_ptr, (current_size + 1) * sizeof(code_conv));
    if (*code_ptr == NULL) {
        display_internal_error(ERROR_CODE_1);
        free(old_ptr);
        return 0;
    }
    return 1;
}


unsigned short convert_command_to_short(command_parts *cmd) {
    unsigned short src_bits, op_bits, dest_bits;
    src_bits = op_bits = dest_bits = 0;

    /* Check if the source is a register and set the corresponding bits. */
    if (identify_register(cmd->source) >= 0) {
        src_bits = ADDRESSING_IS_REG << SHIFT_TO_SOURCE;  /* Fill in register addressing bits. */
    }
    /* Check if the source is a legal label and set the corresponding bits. */
    else if (validate_label(cmd->source)) {
        src_bits = (short)(ADDRESSING_IS_LABEL << SHIFT_TO_SOURCE);  /* Fill in label addressing bits. */
    }
    /* Check if the source is a numerical value and set the corresponding bits. */
    else if (is_number_with_prefix(cmd->source)) {
        src_bits = (short)(ADDRESSING_IS_DIRECT << SHIFT_TO_SOURCE);  /* Fill in direct addressing bits. */
    }
    /* Check if the source is a pointer and set the corresponding bits. */
    else if (is_valid_pointer(cmd->source)) {
        src_bits = (short)(ADDRESSING_IS_POINTER << SHIFT_TO_SOURCE); /* Fill in pointer addressing bits */
    }

    /* Check if the destination is a register and set the corresponding bits. */
    if (identify_register(cmd->dest) >= 0) {
        dest_bits = (short)(ADDRESSING_IS_REG << SHIFT_TO_DEST);  /* Fill in register addressing bits. */
    }
    /* Check if the destination is a legal label and set the corresponding bits. */
    else if (validate_label(cmd->dest)) {
        dest_bits = (short)(ADDRESSING_IS_LABEL << SHIFT_TO_DEST);  /* Fill in label addressing bits. */
    }
    /* Check if the destination is a numerical value and set the corresponding bits. */
    else if (is_number_with_prefix(cmd->dest)) {
        dest_bits = (short)(ADDRESSING_IS_DIRECT << SHIFT_TO_DEST);  /* Fill in direct addressing bits. */
    }
    /* Check if the destination is a pointer and set the corresponding bits. */
    else if (is_valid_pointer(cmd->dest)) {
        dest_bits = (short)(ADDRESSING_IS_POINTER << SHIFT_TO_DEST); /* Fill in pointer addressing bits */
    }

    /* Set the opcode bits. */
    op_bits = (short)(cmd->opcode) << SHIFT_TO_OPCODE;  /* Fill in opcode bits. */

    /* Combine the source, opcode, and destination bits to obtain the final unsigned short representation. */
    return (src_bits | op_bits | dest_bits);
}


unsigned short convert_reg_or_pointer_to_short(command_parts *cmd, int is_source) {
    static int handled;
    int reg1, reg2;
    unsigned short src_bits, dest_bits;
    src_bits = dest_bits = 0;

    if (is_source) {
        /* if the register/pointer we are converting is a source argument */
        if ((reg1 = identify_register(cmd->source)) >= 0) {
            src_bits = reg1 << SHIFT_TO_SOURCE_REG;
        }
        if ((reg1 = identify_pointer(cmd->source)) >= 0) {
            src_bits = reg1 << SHIFT_TO_SOURCE_REG;
        }
        /* if the dest argument is also a register/pointer */
        if ((reg2 = identify_pointer(cmd->dest)) >= 0) {
            dest_bits = reg2 << DEST_BITS_SHIFT_REG;
        }
        if ((reg2 = identify_register(cmd->dest)) >= 0) {
            dest_bits = reg2 << DEST_BITS_SHIFT_REG;
        }
        handled = 1; /* Indicating source & dest register/pointers have been dealt with or dest is not a register/pointer */
        /* return the combined value of both source & dest registers/pointer
         * if dest wasn't a register/pointer, then '|' with zero doesn't change */
        return (src_bits | dest_bits);
    }
    /* Dealing with dest register/pointer, checking we didn't deal with it before we need to check if it's a register/pointer */
    else if (handled == 0) {
        if ((reg2 = identify_register(cmd->dest)) >= 0) {
            dest_bits = reg2 << DEST_BITS_SHIFT_REG;
        }
        else if ((reg2 = identify_pointer(cmd->dest)) >= 0) {
            dest_bits = reg2 << DEST_BITS_SHIFT_REG;
        }
        return dest_bits;
    }

    handled = 0; /* resetting the static variable to zero for next line */
    /* we dealt with the dest register/pointer already so we return a value to indicate that and avoid doing it again */
    return DOUBLE_REGS_VALUE;
}

int insert_machine_code_line(code_conv **code_ptr, unsigned short value, char *label_str, int *instruction_counter, location asm_location) {
    /* Check if memory allocation increase for the code_conv array succeeded */
    if (expand_code_memory(code_ptr, *instruction_counter) == 0) {
        return 0;  /* Return 0 if memory allocation fails */
    }

    /* Set the numerical value of the assembly line */
    (*code_ptr + *instruction_counter)->short_num = value;

    /* Set the assembly line number */
    (*code_ptr + *instruction_counter)->assembly_line = asm_location.line_num;

    /* Check if a label is available in this line, and handle memory allocation for it. */
    if (label_str == NULL) {
        (*code_ptr + *instruction_counter)->label = NULL;  /* No label provided; set label to NULL. */
    } else {
        (*code_ptr + *instruction_counter)->label = handle_malloc((strlen(label_str) + 1) * sizeof(char));
        if ((*code_ptr + *instruction_counter)->label == NULL) {
            return 0;  /* Return 0 if memory allocation for the label fails. */
        }
        strcpy((*code_ptr + *instruction_counter)->label, label_str);  /* Copy the label string to the allocated memory. */
    }

    return 1;  /* Return 1 to indicate successful addition of the machine code line. */
}


int insert_extra_machine_code_line(code_conv **code_ptr, command_parts *cmd, int *instruction_counter, int is_source, location asm_location) {
    unsigned short value;
     char *argument;
   
    /* Determine the argument based on whether the source or destination operand is being processed */
    argument = (is_source) ? cmd->source : cmd->dest;

    /* Check if the argument passed is a register or pointer, if yes set a numerical value for it,
     * The last condition checks if both source and dest are registers/pointers and therefore have been dealt with together */
    if ((identify_register(argument) > 0 || identify_pointer(argument) > 0) && (value = convert_reg_or_pointer_to_short(cmd, is_source)) != DOUBLE_REGS_VALUE) {
        (*instruction_counter)++;
        /* Add the machine code line for the register or pointer representation and masking 0-2 to be 100 */
        if (insert_machine_code_line(code_ptr, value | 4, NULL, instruction_counter, asm_location) == 0) {
            return 0;
        }
    }
    
    /* If argument is not a register or pointer, check if it's a legal label and encode it */
    else if (validate_label(argument)) {
        (*instruction_counter)++;
        if (insert_machine_code_line(code_ptr, 2, argument, instruction_counter, asm_location) == 0) {
            return 0;
        }
    }
    
    /* Check if the argument is a numerical value */
    else if (is_number_with_prefix(argument)) {
        (*instruction_counter)++;
        /* representing number in 3-14 bits, therefore pushing the number ARE_BITS bits to the left and masking 0-2 to be 100 */
        if (insert_machine_code_line(code_ptr, atoi(argument + 1) << ARE_BITS | 4, NULL, instruction_counter, asm_location) == 0) {
            return 0;
        }
    }
    
    /* added successfully or had nothing to add */
    return 1;
}




int insert_machine_code_data(code_conv **data_ptr, inst_parts *instruction, int *data_counter, location asm_location) {
    int i;
    int instruction_length = instruction->len;

    /* Check if data pointer is NULL before proceeding */
    if (data_ptr == NULL || *data_ptr == NULL) {
        /* Handle the error appropriately */
        return 0;
    }

    for (i = 0; i < instruction_length; i++) {
        /* Check if memory allocation for the code_conv array succeeded */
        if (expand_code_memory(data_ptr, *data_counter) == 0) {
            return 0;
        }

        /* Set the numerical value in the current code_conv entry */
        (*data_ptr + *data_counter)->short_num = *(instruction->nums + i);

        /* A data line cannot include a label as an argument */
        (*data_ptr + *data_counter)->label = NULL;

        /* Set the assembly line number associated with the code_conv */
        (*data_ptr + *data_counter)->assembly_line = asm_location.line_num;

        (*data_counter)++;
    }
    
    return 1;
}


/*Helper method to print binary code to user*/
void print_binary_code(code_conv *code,int IC_len){
    int i;
    char *bin_num;
    for (i = 0; i <= IC_len; i++){
        bin_num = convert_short_to_binary((code + i)->short_num);
        printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (code + i)->assembly_line,+IC_INIT_VALUE+i, bin_num);
    }
}



int merge_machine_code_and_data(code_conv **code_ptr, code_conv *data_ptr, int instruction_count, int data_count) {
    int i;
    code_conv *original_code = *code_ptr;

    /* Check if memory allocation for the code_conv array succeeded */
    if (expand_code_memory(code_ptr, instruction_count + data_count) == 0) {
        free(original_code);
        free(data_ptr);
        return 0;
    }

    /* Copying the info from the data lines into the end of the command code lines */
    for (i = 0; i < data_count; i++) {
        (*code_ptr + instruction_count + i + 1)->label = (data_ptr + i)->label;
        (*code_ptr + instruction_count + i + 1)->assembly_line = (data_ptr + i)->assembly_line;
        (*code_ptr + instruction_count + i + 1)->short_num = (data_ptr + i)->short_num;
    }
    free(data_ptr); /* No need anymore for the code from the data */
    
    return 1; /* Return 1 to indicate successful merge of machine code and data */
}


void free_code(code_conv *code, int code_count) {
    int i;

    /* Iterate through the code_conv array and free memory for label strings */
    for (i = 0; i <= code_count; i++) {
        if ((code + i)->label != NULL) {
            free((code + i)->label);
        }
    }

    /* Free memory for the entire code_conv array */
    free(code);
}
