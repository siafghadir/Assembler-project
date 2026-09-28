#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "helper_functions.h"
#include "Errors.h"

/* Memory Management Utility Function */
void *handle_malloc(long object_size) {
    void *object_ptr = malloc(object_size);
    if (object_ptr == NULL) {
        display_internal_error(ERROR_CODE_1);
    }
    return object_ptr;
}

/* Mathematical Utility Function for Two's Complement Calculation */
unsigned short calculate_twos_complement(unsigned short number) {
    return ~number + 1;
}

/* Conversion Function to Convert Short to Binary */
char *convert_short_to_binary(unsigned short number) {
    char *binary_representation;
    int bit_position;

    if ((short)number < 0) {
        number = calculate_twos_complement(number);
    }

    binary_representation = (char *)malloc((WORD_LEN + 1) * sizeof(char));
    if (binary_representation == NULL) {
        display_internal_error(ERROR_CODE_1);
        return NULL;
    }

    for (bit_position = 0; bit_position < WORD_LEN; bit_position++) {
        binary_representation[bit_position] = '0';
    }
    binary_representation[WORD_LEN] = '\0';

    for (bit_position = WORD_LEN - 1; number != 0; bit_position--) {
        binary_representation[bit_position] = (number % 2) + '0';
        number /= 2;
    }

    return binary_representation;
}

/* Conversion Function to Convert Short to Octal */
char *convert_short_to_octal(unsigned short number) {
    char *octal_representation = (char *)malloc(6 * sizeof(char));
    int i;

    if (octal_representation == NULL) {
        return NULL;
    }

    for (i = 0; i < 5; i++) {
        octal_representation[4 - i] = (number & 0x7) + '0';
        number >>= 3;
    }

    octal_representation[5] = '\0';

    return octal_representation;
}

/* File Management Utility Function to Add a New File */
char *add_new_file(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = handle_malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(new_file_name, file_name);

    if ((c = strchr(new_file_name, '.')) != NULL) {
        *c = '\0';
    }
    strcat(new_file_name, ending);
    return new_file_name;
}

/* File Management Utility Function to Copy a File */
int copy_file(char *file_name_dest, char *file_name_orig) {
    char str[MAX_LINE_LENGTH];
    FILE *fp, *fp_dest;

    fp = fopen(file_name_orig, "r");
    if (fp == NULL) {
        display_internal_error(ERROR_CODE_8);
        return 0;
    }

    fp_dest = fopen(file_name_dest, "w");
    if (fp_dest == NULL) {
        display_internal_error(ERROR_CODE_7);
        fclose(fp);
        return 0;
    }

    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        fprintf(fp_dest, "%s", str);
    }

    fclose(fp);
    fclose(fp_dest);
    return 1;
}

/* File Management Utility Function for Abrupt Close */
void abrupt_close(int num_args, ...) {
    int i;
    char *str;
    FILE *fp;
    va_list args;

    va_start(args, num_args);
    for (i = 0; i < num_args; i++) {
        if (strcmp(va_arg(args, char*), "%s") == 0) {
            i++;
            str = va_arg(args, char*);
            remove(str);
            free(str);
        } else {
            fp = va_arg(args, FILE*);
            fclose(fp);
        }
    }
    va_end(args);
}
