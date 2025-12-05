/*

C-ploration 10 for CS 271 
[NAME] Benny Garcia
[TERM] FALL 2025
  
*/

#include "parser.h"
#include "error.h"
#include "symtable.h"

#define MAX_INSTRUCTION_COUNT 30000

int main(int argc, const char *argv[]) {

    if (argc != 2) {
        exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    }

   instruction *instructions = malloc(MAX_INSTRUCTION_COUNT * sizeof(instruction));
   if(instructions == NULL) {
        printf("ERROR: Could not allocate memory for instructions.\n");
        return EXIT_FAILURE;
   }

   int num_instructions = parse(file, instructions);

    /*symtable_print_labels(); */

    fclose(file);

    assemble(argv[1], instructions, num_instructions)

    free(instructions);

    return EXIT_SUCCESS;
}


