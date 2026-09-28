#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "project_constants.h"
#include "Errors.h"
#include "encoding.h"
#include "helper_functions.h"

/* Utility Function to free the memory allocated for the label table */
void free_label_table(label_address *label_table, int label_table_line) {
    int i;
    for (i = 0; i < label_table_line; i++) {
        free((label_table + i)->label_name);
    }
    free(label_table);
}

/* Utility Function to free the memory allocated for the other tables */
void free_other_table(other_table *table, int count) {
    int i;
    for (i = 0; i < count; i++) {
        free((table + i)->label_name);
    }
    free(table);
}

/* Function to add additional labels to the table */
int add_additional_labels(other_table **table_ref, int current_count, inst_parts *instruction, location file_loc, int *error_status) {
    other_table *current_table;
    int name_length;

    if (instruction->arg_label == NULL) {
        return 0;
    }

    current_table = *table_ref;
    (current_table + current_count - 1)->assembly_line = file_loc.line_num;
    name_length = strlen(instruction->arg_label) + 1;

    (current_table + current_count - 1)->label_name = (char *)malloc(name_length * sizeof(char));
    if ((current_table + current_count - 1)->label_name == NULL) {
        *error_status = ERROR_CODE_1;
        return 0;
    }

    strcpy((current_table + current_count - 1)->label_name, instruction->arg_label);

    *table_ref = realloc(*table_ref, (current_count + 1) * sizeof(other_table));
    if (*table_ref == NULL) {
        *error_status = ERROR_CODE_1;
        free(current_table);
        return 0;
    }

    return 1;
}

/* Function to add a label to the label table */
int add_label_to_table(label_address **label_table_ref, int num_lines, char *label_name, int address_counter, location file_loc, 
                       int data_line_flag, int *error_status) {
    label_address *temp_table;

    temp_table = *label_table_ref;
    *label_table_ref = realloc(*label_table_ref, num_lines * sizeof(label_address));
    if (*label_table_ref == NULL) {
        *error_status = ERROR_CODE_1;
        free(temp_table);
        return 0;
    }

    (*label_table_ref + num_lines - 1)->is_data_line = data_line_flag;
    (*label_table_ref + num_lines - 1)->address = address_counter;
    (*label_table_ref + num_lines - 1)->assembly_line = file_loc.line_num;

    (*label_table_ref + num_lines - 1)->label_name = (char *)malloc((strlen(label_name) + 1) * sizeof(char));
    if ((*label_table_ref + num_lines - 1)->label_name == NULL) {
        *error_status = ERROR_CODE_1;
        return 0;
    }
    strcpy((*label_table_ref + num_lines - 1)->label_name, label_name);
    /*printf("Label successfully added: %s\n", (*label_table_ref + num_lines - 1)->label_name);*/

    return 1;
}

/* Function to validate that all labels in the table are unique */
int validate_unique_labels(label_address *label_table, int num_labels, char *source_file) {
    int i, j;

    for (i = 0; i < num_labels - 1; i++) {
        for (j = i + 1; j < num_labels; j++) {
            if (strcmp((label_table + i)->label_name, (label_table + j)->label_name) == 0) {
                location error_location;
                error_location.file_name = source_file;
                error_location.line_num = (label_table + j)->assembly_line;
                display_external_error(ERROR_CODE_55, error_location);
                return 0;
            }
        }
    }
    return 1;
}

/* Function to update the label table with the data offset */
void update_label_table_with_data_offset(label_address *label_table, int num_labels, int instruction_counter) {
    int i;
    for (i = 0; i < num_labels; i++) {
        if ((label_table + i)->is_data_line) {
            (label_table + i)->address += instruction_counter + 1;
        }
    }
}

/* Function to initialize label addresses with the correct value */
void initialize_label_addresses(label_address *label_table, int num_labels) {
    int i;
    for (i = 0; i < num_labels; i++) {
        (label_table + i)->address += IC_INIT_VALUE;
    }
}

/* Function to update extern labels in the code array */
void update_extern_labels(code_conv *code_array, other_table *extern_table, int num_externs, int code_count, char *source_file) {
    int i, j;
    int is_found;

    for (i = 0; i <= code_count; i++) {
        is_found = 0;
        if ((code_array + i)->label != NULL) {
            for (j = 0; j < num_externs && !is_found; j++) {
                if (strcmp((code_array + i)->label, (extern_table + j)->label_name) == 0) {
                    (code_array + i)->short_num -= 1;
                    is_found = 1;
                }
            }
        }
    }
}

/* Function to check if an extern is defined within the assembly file */
int check_extern_defined(other_table *extern_entries, int num_externs, label_address *label_entries, int num_labels, 
                          char *source_file) {
    int i, j;
    int is_found, is_extern_defined = 0;
    location error_loc;

    for (i = 0; i < num_externs; i++) {
        is_found = 0;
        for (j = 0; j < num_labels && !is_found; j++) {
            if (strcmp((extern_entries + i)->label_name, (label_entries + j)->label_name) == 0) {
                is_extern_defined = 1;
                is_found = 1;
                error_loc.file_name = source_file;
                error_loc.line_num = (label_entries + j)->assembly_line;
                display_external_error(ERROR_CODE_56, error_loc);
            }
        }
    }
    return is_extern_defined;
}

/* Function to update the code array with label addresses */
int update_code_with_labels(code_conv *code_array, label_address *label_entries, int num_labels, int code_length, char *source_file) {
    int i, j;
    int is_found, has_error = 0;

    for (i = 0; i <= code_length; i++) {
        is_found = 0;

        if ((code_array + i)->label != NULL && (code_array + i)->short_num != 1) {
            /*printf("Verifying label in code: %s\n", (code_array + i)->label);*/

            for (j = 0; j < num_labels && !is_found; j++) {
                if (strcmp((code_array + i)->label, (label_entries + j)->label_name) == 0) {
                    (code_array + i)->short_num |= ((label_entries + j)->address) << ARE_BITS;
                    is_found = 1;
                }
            }

            if (!is_found) {
                location error_loc;
                error_loc.file_name = source_file;
                error_loc.line_num = (code_array + i)->assembly_line;
                display_external_error(ERROR_CODE_36, error_loc);
                has_error = 1;
            }
        }
    }

    return !has_error;
}

/* Function to generate the externs file */
int generate_externs_file(code_conv *code_entries, int num_entries, other_table *extern_labels, int num_externs, char *source_file) {
    FILE *extern_file;
    int i, j;
    int is_found, is_empty;
    char *temp_file_name;

    temp_file_name = add_new_file(source_file, ".ext");
    extern_file = fopen(temp_file_name, "w");

    is_empty = 1;

    if (extern_file == NULL) {
        display_internal_error(ERROR_CODE_7);
        return 0;
    }

    for (i = 0; i <= num_entries; i++) {
        is_found = 0;
        if ((code_entries + i)->label != NULL) {
            for (j = 0; j < num_externs && !is_found; j++) {
                if (strcmp((code_entries + i)->label, (extern_labels + j)->label_name) == 0) {
                    fprintf(extern_file, "%s\t%d\n", (extern_labels + j)->label_name, IC_INIT_VALUE + i);
                    is_found = 1;
                    is_empty = 0;
                }
            }
        }
    }
    fclose(extern_file);

    if (is_empty) {
        remove(temp_file_name);
    }

    free(temp_file_name);

    return 1;
}

/* Function to write entry labels to the file */
int write_entry_labels_to_file(label_address *label_table, int num_labels, other_table *entry_table, int num_entries, char *source_file) {
    FILE *entry_file;
    int i, j;
    int is_found, is_empty;
    char *temp_file_name;

    temp_file_name = add_new_file(source_file, ".ent");
    entry_file = fopen(temp_file_name, "w");

    if (entry_file == NULL) {
        display_internal_error(ERROR_CODE_7);
        return 0;
    }

    is_empty = 1;

    for (i = 0; i < num_labels; i++) {
        is_found = 0;

        for (j = 0; j < num_entries && !is_found; j++) {
            if (strcmp((label_table + i)->label_name, (entry_table + j)->label_name) == 0) {
                fprintf(entry_file, "%s\t%d\n", (entry_table + j)->label_name, (label_table + i)->address);
                is_found = 1;
                is_empty = 0;
            }
        }
    }

    fclose(entry_file);

    if (is_empty) {
        remove(temp_file_name);
    }

    free(temp_file_name);

    return 1;
}

#endif
