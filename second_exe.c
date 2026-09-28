#include <stdio.h>
#include "second_exe.h"
#include "table.h"
#include "Errors.h"
#include "helper_functions.h"

/* Utility Function to free all allocated memory */
void free_all_memory(code_conv *code, label_address *label_table, other_table *entries, other_table *externs, \
    int code_count, int label_table_line, int entries_count, int externs_count) {
    free_code(code, code_count);
    free_label_table(label_table, label_table_line);
    free_other_table(entries, entries_count);
    free_other_table(externs, externs_count);
}

/* Core Function to convert code to octal and write to output file */
int convert_code_to_octal(code_conv *code, int count, char *file_name, int IC, int DC) {
    int i;
    int address = 100;
    FILE *fp;
    char *output_file_name, *octal_string;

    output_file_name = add_new_file(file_name, ".ob");

    fp = fopen(output_file_name, "w");
    if (fp == NULL) {
        display_internal_error(ERROR_CODE_7);
        return 0;
    }

    fprintf(fp, "%d %d\n", IC + 1, DC);

    for (i = 0; i <= count; i++) {
        octal_string = convert_short_to_octal((code + i)->short_num);
        fprintf(fp, "%04d %s\n", address, octal_string);
        address++;
        free(octal_string);
    }

    free(output_file_name);
    fclose(fp);

    return 1;
}

/* Main Function to execute the second pass */
int execute_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_count,
                        int externs_count, int entries_count, code_conv *code, code_conv *data,
                        other_table *externs, other_table *entries, int error_found) {

    if (IC > IC_MAX) {
        display_internal_error(ERROR_CODE_54);
        error_found = 1;
    }

    if (!validate_unique_labels(label_table, label_table_count, file_name)) {
        error_found = 1;
    }

    if (check_extern_defined(externs, externs_count, label_table, label_table_count, file_name)) {
        error_found = 1;
    }

    update_label_table_with_data_offset(label_table, label_table_count, IC);
    initialize_label_addresses(label_table, label_table_count);

    if (!merge_machine_code_and_data(&code, data, IC, DC)) {
        error_found = 1;
    }

    update_extern_labels(code, externs, externs_count, IC + DC, file_name);

    if (!update_code_with_labels(code, label_table, label_table_count, IC, file_name)) {
        error_found = 1;
    }

    /*print_binary_code(code, IC + DC);*/

    if (!error_found) {
        convert_code_to_octal(code, IC + DC, file_name, IC, DC);
        generate_externs_file(code, IC + DC, externs, externs_count, file_name);
        write_entry_labels_to_file(label_table, label_table_count, entries, entries_count, file_name);
    }

    free_all_memory(code, label_table, entries, externs, IC + DC, label_table_count, entries_count, externs_count);

    return !error_found;
}
