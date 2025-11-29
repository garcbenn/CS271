/****************************************
 * C-ploration 9 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"
#include "error.h"
#include "symtable.h"

int main(int argc, const char *argv[]) {

    if (argc != 2) {
        exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    }

    parse(file);

    symtable_print_labels();

    fclose(file);
    return EXIT_SUCCESS;
}


