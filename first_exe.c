/* Contains major function that are related to the first pass */
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

/* Utility Function to handle memory allocation for externs, entries, code, and data */
int handle_allocation(other_table **externs, other_table **entries, code_conv **code, code_conv **data) {
    int error_found = 0;

    *externs = handle_malloc(sizeof(other_table));
    if (*externs == NULL) {
        return 1;
    }

    *entries = handle_malloc(sizeof(other_table));
    if (*entries == NULL) {
        return 1;
    }

    *code = handle_malloc(sizeof(code_conv));
    if (*code == NULL) {
        return 1;
    }

    *data = handle_malloc(sizeof(code_conv));
    if (*data == NULL) {
        return 1;
    }

    return error_found;
}

/* Main Function to execute the first pass */
int exe_first_pass(char *file_name) {
    int error_code = 0, IC = -1, DC = 0, error_found = 0, label_table_line = 0;
    int externs_count = 0, entries_count = 0, inst_created = 1;

    code_conv *code = NULL, *data = NULL;
    other_table *externs = NULL, *entries = NULL;
    command_parts *command = NULL;
    inst_parts *inst = NULL;
    location am_file;
    char str[MAX_LINE_LENGTH], str_copy[MAX_LINE_LENGTH];
    FILE *fp = NULL;
    label_address *label_table = NULL;

    am_file.file_name = file_name;
    am_file.line_num = 0;

    if (check_line_length(file_name)) {
        error_found = 1;
    }

    fp = fopen(file_name, "r");

    error_found = handle_allocation(&externs, &entries, &code, &data);

    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL && IC + DC <= IC_MAX - IC_INIT_VALUE) {
        error_code = 0;
        am_file.line_num++;

        if (strcmp(str, "\n") == 0) continue;

        if (strchr(str, '.')) {
            strcpy(str_copy, str);

            if (strstr(str_copy, ".entry") || strstr(str_copy, ".extern")) {
                inst = parse_entry_or_extern(str_copy, &error_code);

                if (inst->is_extern) {
                    add_additional_labels(&externs, ++externs_count, inst, am_file, &error_code);
                } else {
                    add_additional_labels(&entries, ++entries_count, inst, am_file, &error_code);
                }

            } else if (strstr(str_copy, ".data") || strstr(str_copy, ".string")) {
                inst = parse_instruction(str_copy, &error_code);

                if (inst->label) {
                    add_label_to_table(&label_table, ++label_table_line, inst->label, DC, am_file, 1, &error_code);
                }

            } else {
                inst_created = 0;
                error_code = ERROR_CODE_58;
            }

            if (error_code) {
                display_external_error(error_code, am_file);
                if (inst_created) free(inst);
                error_found = 1;
                continue;
            } else if (inst_created && insert_machine_code_data(&data, inst, &DC, am_file) == 0) {
                error_found = 1;
                continue;
            }

            if (inst_created) {
                if (inst->nums) free(inst->nums);
                free(inst);
            }

        } else {
            command = parse_command(str, &error_code);

            if (error_code == 0) {
                IC++;
                if (command && command->label) {
                    add_label_to_table(&label_table, ++label_table_line, command->label, IC, am_file, 0, &error_code);
                }
            } else {
                display_external_error(error_code, am_file);
                free(command);
                error_found = 1;
                continue;
            }

            if (insert_machine_code_line(&code, convert_command_to_short(command) + 4, NULL, &IC, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }

            if (insert_extra_machine_code_line(&code, command, &IC, 1, am_file) == 0 ||
                insert_extra_machine_code_line(& code, command, &IC, 0, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }

            free(command);
        }
    }

    if (execute_second_pass(file_name, label_table, IC, DC, label_table_line, externs_count, entries_count, code, data,
                        externs, entries, error_found) == 0) {
        error_found = 1;
    }

    fclose(fp);
    return error_found;
}
