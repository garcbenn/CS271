/****************************************
 * C-ploration 7 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }       

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    parse(file);

    symtable_print_labels();

    fclose(file);
    return EXIT_SUCCESS;        
}


