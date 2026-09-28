#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "preproces.h"
#include "helper_functions.h"
#include "project_constants.h"
#include "deal_with_text.h"
#include "Errors.h"

/* Utility Function to check if the char is a space or a tab */
int is_space_or_tab(char c) {
    return (isspace(c) && c != '\n');
}

/* Utility Function to trim spaces around commas in a string */
void trim_spaces_around_comma(char *str) {
    char *current_pos = str;

    /* If the line starts with a comma, exit to avoid errors */
    if (*current_pos == ',') {
        return;
    }

    /* Loop through the string to find commas */
    while ((current_pos = strchr(current_pos, ',')) != NULL) {
        /* Check and remove space before the comma */
        if (*(current_pos - 1) == ' ') {
            memmove(current_pos - 1, current_pos, strlen(current_pos) + 1);
            /* If there's also a space after the comma, remove it */
            if (*(current_pos) == ' ') {
                memmove(current_pos, current_pos + 1, strlen(current_pos + 1) + 1);
            }
        }
        /* Check and remove space after the comma */
        else if (*(current_pos + 1) == ' ') {
            memmove(current_pos + 1, current_pos + 2, strlen(current_pos + 2) + 1);
            current_pos++;
        } else {
            current_pos++;
        }
    }
}

/* String Manipulation Function to trim extra spaces from a string */
void trim_extra_spaces(char str[]) {
    int i = 0, j = 0;
    char temp_str[MAX_LINE_LENGTH];

    /* Skip initial white spaces at the beginning of the line */
    while (is_space_or_tab(str[i])) {
        i++;
    }

    /* Process the rest of the string */
    while (str[i] != '\0') {
        while (!is_space_or_tab(str[i]) && str[i] != '\0') {
            temp_str[j++] = str[i++];
        }

        if (str[i] == '\0') {
            break;
        }

        while (is_space_or_tab(str[i])) {
            i++;
        }

        if (str[i] != '\n' && str[i] != '\0') {
            temp_str[j++] = ' ';
        }
    }

    /* Copy the terminating character */
    temp_str[j] = str[i];
    temp_str[j + 1] = '\0';

    /* Remove spaces adjacent to commas */
    trim_spaces_around_comma(temp_str);

    /* Copy the modified string back to the original */
    strcpy(str, temp_str);
}

/* Core Functionality to extract text from a file */
char *extract_text(FILE *file_ptr, fpos_t *position, int length) {
    int i;
    char *text_buffer;

    if (fsetpos(file_ptr, position) != 0) {
        printf("Failed to reset file position in extract_text\n");
        return NULL;
    }

    text_buffer = handle_malloc((length + 1) * sizeof(char));

    for (i = 0; i < length; i++) {
        text_buffer[i] = getc(file_ptr);
    }
    text_buffer[i] = '\0';

    fgetpos(file_ptr, position);

    return text_buffer;
}

/* Core Functionality to clean file spaces */
char *clean_file_spaces(char file_name[]) {
    char *temp_file_name;
    char buffer[BIG_NUMBER_CONST];
    int line_counter;
    FILE *input_file, *output_file;

    input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        display_internal_error(ERROR_CODE_2);
        return NULL;
    }

    temp_file_name = add_new_file(file_name, ".t01");
    if (temp_file_name == NULL) {
        abrupt_close(2, "file", input_file);
        return NULL;
    }

    output_file = fopen(temp_file_name, "w");
    if (output_file == NULL) {
        abrupt_close(4, "file", input_file, "%s", temp_file_name);
        display_internal_error(ERROR_CODE_7);
        return NULL;
    }

    line_counter = 0;
    while (fgets(buffer, sizeof(buffer), input_file) != NULL) {
        line_counter++;
        if (strlen(buffer) > MAX_LINE_LENGTH) {
            location error_loc;
            error_loc.file_name = file_name;
            error_loc.line_num = line_counter;
            display_external_error(ERROR_CODE_30, error_loc);
            fclose(input_file);
            fclose(output_file);
            return NULL;
        } else if (buffer[0] == ';') {
            buffer[0] = '\n';
            buffer[1] = '\0';
        } else {
            trim_extra_spaces(buffer);
        }

        fprintf(output_file, "%s", buffer);
    }

    fclose(input_file);
    fclose(output_file);

    return temp_file_name;
}
