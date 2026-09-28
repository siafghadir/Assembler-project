#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preproces.h"
#include "helper_functions.h"
#include "project_constants.h"
#include "deal_with_text.h"
#include "Errors.h"
#include "lexer.h"

/* Utility Function to capture the content of a macro */
char *capture_macro_content(FILE *fp, fpos_t *start_pos, int *line_counter) {
    int macro_length = 0;
    char *macro_content;
    char buffer[MAX_LINE_LENGTH];

    if (fsetpos(fp, start_pos) != 0) {
        display_internal_error(ERROR_CODE_11);
        return NULL;
    }
    buffer[0] = '\0';

    while (fgets(buffer, MAX_LINE_LENGTH, fp) && strcmp(buffer, "endmcro\n") != 0) {
        if (strstr(buffer, "endmcro") != NULL && strlen(buffer) != strlen("endmcro")) {
            display_internal_error(ERROR_CODE_12);
            return NULL;
        }
        (*line_counter)++;
        if (strcmp(buffer, "endmcro\n") != 0) {
            macro_length += strlen(buffer);
        }
    }

    macro_content = extract_text(fp, start_pos, macro_length);
    return macro_content;
}

/* Utility Function to validate the declaration of a macro */
int validate_macro_declaration(char *str, char **name, int line_count, char *file_name) {
    char *macro_name, *extra_text;

    macro_name = strtok(NULL, " \n");
    if (macro_name == NULL) {
        display_internal_error(ERROR_CODE_9);
        return 0;
    }

    if (is_instruction(macro_name) || identify_opcode(macro_name) >= 0 || identify_register(macro_name) >= 0) {
        location error_loc;
        error_loc.file_name = file_name;
        error_loc.line_num = line_count;
        display_external_error(ERROR_CODE_17, error_loc);
        return 0;
    }

    extra_text = strtok(NULL, "\n");
    if (extra_text != NULL) {
        display_internal_error(ERROR_CODE_10);
        return 0;
    }

    *name = handle_malloc((strlen(macro_name) + 1) * sizeof(char));
    strcpy(*name, macro_name);

    return 1;
}

/* Function to process macros in a file */
int process_macros(char *file_name, node **head) {
    int line_count, is_successful;
    FILE *fp;
    char buffer[MAX_LINE_LENGTH];
    char *macro_name, *macro_content;
    fpos_t current_pos;

    is_successful = 1;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        display_internal_error(ERROR_CODE_8);
        is_successful = 0;
        return is_successful;
    }

    line_count = 0;
    while (fgets(buffer, MAX_LINE_LENGTH, fp)) {
        line_count++;

        if (strcmp(strtok(buffer, " "), "mcro") == 0) {
            int macro_start_line = line_count;
            if (!validate_macro_declaration(buffer, &macro_name, line_count, file_name)) {
                is_successful = 0;
                continue;
            }

            fgetpos(fp, &current_pos);
            macro_content = capture_macro_content(fp, &current_pos, &line_count);
            if (macro_content == NULL) {
                is_successful = 0;
                continue;
            }

            fsetpos(fp, &current_pos);
            append_to_list(head, macro_name, macro_content, macro_start_line);
        }
    }

    fclose(fp);
    return is_successful;
}

/* Function to remove macro declarations from a file */
char *remove_macro_declarations(char *file_name) {
    char *token, *new_file_name;
    char buffer[MAX_LINE_LENGTH];
    char buffer_copy[MAX_LINE_LENGTH];
    FILE *input_file, *output_file;

    input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        display_internal_error(ERROR_CODE_8);
        return NULL;
    }

    new_file_name = add_new_file(file_name, ".t02");

    output_file = fopen(new_file_name, "w");
    if (output_file == NULL) {
        display_internal_error(ERROR_CODE_7);
        abrupt_close(4, "file", input_file, "%s", new_file_name);
        return NULL;
    }

    while (fgets(buffer, MAX_LINE_LENGTH, input_file)) {
        strcpy(buffer_copy, buffer);
        token = strtok(buffer, " \n");

        if (token == NULL) {
            fprintf(output_file, "\n");
            continue;
        }

        if (strcmp(token, "mcro") == 0) {
            while (strcmp(token, "endmcro") != 0) {
                fprintf(output_file, "\n");
                fgets(buffer, MAX_LINE_LENGTH, input_file);
                token = strtok(buffer, " \n");

                while (token == NULL) {
                    fprintf(output_file, "\n");
                    fgets(buffer, MAX_LINE_LENGTH, input_file);
                    token = strtok(buffer, " \n");
                }
            }
            fprintf(output_file, "\n");
        } else {
            fprintf(output_file, "%s", buffer_copy);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return new_file_name;
}

/* Function to replace a macro with its content in a string */
char *replace_macro_with_content(char *str, node *macro) {
    char *macro_pos, *result_str;
    char str_start[MAX_LINE_LENGTH];
    char str_finish[MAX_LINE_LENGTH];

    strcpy(str_start, str);

    macro_pos = strstr(str_start, macro->name);
    *macro_pos = '\0';

    strcpy(str_finish, macro_pos + strlen(macro->name));

    result_str = handle_malloc((strlen(str_start) + strlen(macro->content) + strlen(str_finish) + 1) * sizeof(char));
    if (result_str == NULL) {
        return NULL;
    }

    strcpy(result_str, str_start);
    strcat(result_str, macro->content);
    strcat(result_str, str_finish);

    return result_str;
}

/* Function to replace all macro calls with their content */
char *replace_all_macros(char file_name[], node *head) {
    node *current_macro;
    char *position, *modified_line, *temp_file_name, *final_file_name;
    char buffer[MAX_LINE_LENGTH];
    FILE *temp_file, *final_file;

    temp_file_name = add_new_file(file_name, ".tmp");
    final_file_name = add_new_file(file_name, ".am");

    if (!copy_file(temp_file_name, file_name) || !copy_file(final_file_name, file_name)) {
        display_internal_error(ERROR_CODE_14);
        abrupt_close(4, "%s", temp_file_name, "%s", final_file_name);
        return NULL;
    }

    current_macro = head;
    while (current_macro != NULL) {
        temp_file = fopen(temp_file_name, "r");
        if (temp_file == NULL) {
            display_internal_error(ERROR_CODE_8);
            abrupt_close(4, "%s", temp_file_name, "%s", final_file_name);
            return NULL;
        }

        final_file = fopen(final_file_name, "w");
        if (final_file == NULL) {
            display_internal_error(ERROR_CODE_7);
            abrupt_close(6, "file", temp_file, "%s", temp_file_name, "%s", final_file_name);
            return NULL;
        }

        while (fgets(buffer, MAX_LINE_LENGTH, temp_file)) {
            position = strstr(buffer, current_macro->name);
            if (position != NULL) {
                buffer[strlen(buffer) - 1] = '\0';

                modified_line = replace_macro_with_content(buffer, current_macro);
                if (modified_line == NULL) {
                    abrupt_close(8, "file", final_file, "file", temp_file, "%s", temp_file_name, "%s", final_file_name);
                    return NULL;
                }

                fprintf(final_file, "%s\n", modified_line);
                free(modified_line);
            } else {
                fprintf(final_file, "%s", buffer);
            }
        }

        fclose(temp_file);
        fclose(final_file);

        current_macro = current_macro->next;
        if (current_macro == NULL) {
            break;
        }

        remove(temp_file_name);
        rename(final_file_name, temp_file_name);
    }

    remove(temp_file_name);
    free(temp_file_name);

    return final_file_name;
}

/* Function to check if a macro call appears before its declaration */
int check_macro_call_before_declaration(char file_name[], node *head) {
    FILE *fp;
    int line_count = 0, error_found = 0;
    node *current_macro;
    char buffer[MAX_LINE_LENGTH];

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        display_internal_error(ERROR_CODE_8);
        return 1;
    }

    while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
        line_count++;

        if (strstr(buffer, "mcro") != NULL) {
            continue;
        }

        current_macro = head;
        while ((current_macro != NULL) && (current_macro->line < line_count)) {
            current_macro = current_macro->next;
        }

        if (current_macro == NULL) {
            continue;
        }

        while (current_macro != NULL) {
            if (strstr(buffer, current_macro->name) != NULL) {
                display_internal_error(ERROR_CODE_16);
                error_found = 1;
            }
            current_macro = current_macro->next;
        }
    }
    fclose(fp);

    return error_found;
}

/* Main Function to execute macro processing on a file */
int execute_macro_processing(char file_name[]) {
    node *macro_list;
    char *temp_file1, *temp_file2, *output_file, *temp_name1, *temp_name2;

    temp_file1 = clean_file_spaces(file_name);
    if (temp_file1 == NULL) {
        return 0;
    }

    macro_list = NULL;

    if (!process_macros(temp_file1, &macro_list)) {
        free_list(macro_list);
        abrupt_close(2, "%s", temp_file1);
        return 0;
    }

    if (check_macro_call_before_declaration(temp_file1, macro_list)) {
        free_list(macro_list);
        abrupt_close(2, "%s", temp_file1);
        return 0;
    }

    temp_file2 = remove_macro_declarations(temp_file1);
    if (temp_file2 == NULL) {
        free_list(macro_list);
        abrupt_close(2, "%s", temp_file1);
        display_internal_error(ERROR_CODE_15);
        return 0;
    }

    free(temp_file1);

    output_file = replace_all_macros(temp_file2, macro_list);
    if (output_file == NULL) {
        free_list(macro_list);
        abrupt_close(4, "%s", temp_file2);
        display_internal_error(ERROR_CODE_15);
        return 0;
    }

    temp_name1 = add_new_file(file_name, ".t01");
    temp_name2 = add_new_file(file_name, ".t02");
    remove(temp_name1);
    remove(temp_name2);

    free(temp_name1);
    free(temp_name2);

    free(temp_file2);
    free(output_file);
    free_list(macro_list);

    return 1;
}
