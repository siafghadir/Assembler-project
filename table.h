#ifndef TABLE_H
#define TABLE_H

#include "lexer.h"
#include "project_constants.h"
#include "encoding.h"

/**
 * @brief Represents the address and related information for a label in the assembly code.
 *
 * This structure stores details about a label, including its address, name, the line of assembly where it's defined,
 * and a flag indicating whether the label is associated with a data line.
 */
typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
    int is_data_line;
} label_address;

/**
 * @brief Holds information about non-instruction labels and their corresponding assembly lines.
 *
 * This structure is used to store information about non-instruction labels, such as their name and the assembly line number
 * where they are referenced.
 */
typedef struct other_table {
    char *label_name;
    int assembly_line;
} other_table;

/* Memory Management Functions */
void free_label_table(label_address *label_table, int label_table_line);
void free_other_table(other_table *table, int count);

/* Table Modification Functions */
int add_label_to_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data_line,
                       int *error_code);
int add_additional_labels(other_table **table, int count, inst_parts *inst, location am_file, int *error_code);
int update_code_with_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name);
void update_label_table_with_data_offset(label_address *label_table, int table_lines, int IC);
void initialize_label_addresses(label_address *label_table, int table_lines);

/* Validation Functions */
int validate_unique_labels(label_address *label_table, int lines, char *file_name);
int check_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                         char *file_name);
void update_extern_labels(code_conv *code, other_table *externs, int externs_count, int count, char *file_name);

/* Output Functions */
int generate_externs_file(code_conv *code, int count, other_table *externs, int externs_count, char *file_name);
int write_entry_labels_to_file(label_address *label_table, int label_table_line, other_table *entries, int entries_count,
                               char *file_name);

#endif
