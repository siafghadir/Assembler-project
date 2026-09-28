#include <stdio.h>
#include <stdlib.h>
#include "preproces.h"
#include "first_exe.h"
#include "helper_functions.h"

/**
 * @brief The main function responsible for processing input files.
 *
 * @param argc The count of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns 0 upon successful completion.
 */
int main(int argc, char *argv[]) {
    char *as_file, *am_file;

    /* Loop through each input file provided via command-line arguments. */
    while (--argc > 0) {
        printf("Starting pre-processing...\n");

        /* Create a new filename with the ".as" extension added to the original input filename. */
        as_file = add_new_file(argv[argc], ".as");

        /* Perform the macro processing on the generated ".as" file. */
        if (!execute_macro_processing(as_file)) {
            /* If macro processing fails, skip to the next file. */
            continue;
        }

        printf("Starting first pass...\n");

        /* Create a new filename with the ".am" extension added to the original input filename. */
        am_file = add_new_file(argv[argc], ".am");

        /* Run the first pass on the generated ".am" file, followed by the second pass if successful. */
        if (exe_first_pass(am_file)) {
            /* If the first pass fails, skip to the next file. */
            continue;
        }

        /* Deallocate the memory used for filenames. */
        free(as_file);
        free(am_file);
    }

    printf("Processing complete.\n");
    return 0;
}
