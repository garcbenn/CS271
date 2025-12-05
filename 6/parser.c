/****************************************
 * C-ploration 10 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"
#include "hack.h"

/* Function: strip
remove whitespace and comments from a line
s: the char* string to strip
returns: the stripped char* string
 */
char *strip(char *s){   

    char s_new[strlen(s)+1];
    int i = 0;

    for (char *s2 = s; *s2; s2++) {
        if (*s2 == '/' && *(s2 +1) == '/')
            break;
        else if (!isspace(*s2))
            s_new[i++] = *s2;
    }

    s_new[i] = '\0';
    strcpy(s, s_new);
    
    return s;   
}

/* Function: is_Atype
 check if the line is an A-type instruction
 line: the char* string to check
 returns: true if A-type, false otherwise
 */
bool is_Atype(const char *line) {
    return line[0] == '@';
}

/* Function: is_label
check if the line is a label
line: the char* string to check
returns: true if label, false otherwise
 */
bool is_label(const char *line) {
    int len = strlen(line);
    if (len < 2) {
        return false;
    }
    return (line[0] == '(' && line[len - 1] == ')');
}

/* Function: is_Ctype
check if the line is a C-type instruction
line: the char* string to check
returns: true if C-type, false otherwise
 */
bool is_Ctype(const char *line) {
    return !is_Atype(line) && !is_label(line);
}

/* Function: parse_A_instruction
Parses an A-type instruction.
line: the full A-instruction string
instr: output a_instruction struct to fill

Returns true if valid, false if invalid label.
 */

bool parse_A_instruction(const char *line, a_instruction *instr) {
    char *s = malloc(strlen(line));
    if (s == NULL) {
        return false;
    }

    strcpy(s, line + 1);
    char *s_end = NULL;
    long result = strtol(s, &s_end, 10);

    if (s == s_end) {
    instr->is_addr = false;
    
    if (!isalpha(s[0]) && s[0] != '_' && s[0] != '.' && s[0] != ':' && s[0] != '$') {
            free(s);
            return false;
        }
    
    instr->label = malloc(strlen(s) + 1);
    strcpy(instr->label, s);

    free(s);
    return true;
}

    if (*s_end != '\0') {
        free(s);
        return false;
    }

    instr->is_addr = true;
    instr->address = (int16_t) result;

    free(s);

    return true;
}

/* Function: parse_C_instruction

Parses a C-type instruction into its components. The full C-instruction string (will be modified by strtok)
instr: output c_instruction struct to fill

*/

void parse_C_instruction(char *line, c_instruction *instr) {
    char *temp = NULL;
    char *jump = NULL;
    char *dest = NULL;
    char *comp = NULL;
    int a = 0;
    
    temp = strtok(line, ";");
    jump = strtok(NULL, ";");
    
    if (temp != NULL) {
        dest = strtok(temp, "=");
        comp = strtok(NULL, "=");
        
        if (comp == NULL) {
            comp = dest;
            dest = NULL;
        }
    }
    
    instr->jump = str_to_jumpid(jump);
    instr->dest = str_to_destid(dest);
    instr->comp = str_to_compid(comp, &a);
    
    instr->a = (a == 0) ? 0 : 1;
}

/*Function: add_predefined_symbols

Loads the 23 predefined Hack symbols into the symbol table before parsing begins.

This function iterates over the predefined_symbols[] array in hack.h, and inserts each symbol name/address pair into the symbol table.
*/

void add_predefined_symbols () {

    for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++) {
        predefined_symbol sym = predefined_symbols[i];
        symtable_insert(sym.name, sym.address);
    }
}

/* Function: parse
iterate each line in the file and strip whitespace and comments. 

file: pointer to FILE to parse
instructions: array to store parsed instructions
returns: number of instructions parsed
 */

int parse(FILE *file, instruction *instructions) {
    char line[MAX_LINE_LENGTH];
    char tmp_line[MAX_LINE_LENGTH];
    unsigned int line_num = 0;
    unsigned int instr_num = 0;

    instruction instr;

    add_predefined_symbols();

    while (fgets(line, sizeof(line), file)) {
        line_num++;

        line[strcspn(line, "\r\n")] = '\0';

        strip(line);

        if (line[0] == '\0') {
            continue;
        }

        if (instr_num >= MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS);
        }

        if (is_label(line)) {
            char label[MAX_LABEL_LENGTH];
            extract_label(line, label);

            if (!isalpha(label[0])) {
                exit_program(EXIT_INVALID_LABEL, line_num, label);
            }

            if (symtable_find(label) != NULL) {
                exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, label);
            }

            symtable_insert(label, instr_num);
            continue;
        } else if (is_Atype(line)) {

            if (!parse_A_instruction(line, &instr.a)) {
                exit_program(EXIT_INVALID_A_INSTR, line_num, line);
            }

            instr.type = A_INSTR;

            if (instr.a.is_addr) {
                /*printf("A: %d\n", instr.a.address);*/
            } else {
                /*printf("A: %s\n", instr.a.label);*/
            }

        } else if (is_Ctype(line)) {
            strcpy(tmp_line, line);

            parse_C_instruction(tmp_line, &instr.c);

            if (instr.c.dest == DEST_INVALID) {
                exit_program(EXIT_INVALID_C_DEST, line_num, line);
            }
            if (instr.c.comp == COMP_INVALID) {
                exit_program(EXIT_INVALID_C_COMP, line_num, line);
            }
            if (instr.c.jump == JMP_INVALID) {
                exit_program(EXIT_INVALID_C_JUMP, line_num, line);
            }

            instr.type = C_INSTR;
            
            /*printf("C: d=%d, c=%d, j=%d\n", instr.c.dest, instr.c.comp, instr.c.jump);*/

        } else {
            instr.type = INVALID_INSTR;
        }

        instructions[instr_num++] = instr;
    }
    return instr_num;
}

opcode instruction_to_opcode(c_instruction instr) {
    opcode op = 0;
    op |= (7 << 13);
    op |= (instr.a << 12);
    op |= (instr.comp << 6);
    op |= (instr.dest << 3);
    op |= instr.jump;

    return op;
}


/* Function: extract_label

Takes a line like "(LABEL)" and copies "LABEL" into label.
line: the input string (must be a label line)
label: output buffer to hold the label text
returns: pointer to label

 */
char *extract_label(const char *line, char *label) {
    const char *start = line + 1;
    const char *end = strchr(start, ')');

    if (end == NULL) {                    
        label[0] = '\0';
        return label;
    }

    size_t len = end - start;
    if (len >= MAX_LABEL_LENGTH)
        len = MAX_LABEL_LENGTH - 1;

    strncpy(label, start, len);
    label[len] = '\0';
    return label;
}

/* Function: assemble
Performs the second pass of the assembler.

file_name: the name of the input .asm file used to generate .hack file
instructions: the array of parsed instructions from pass 1
num_instructions: number of parsed instructions

This function creates the output .hack file, iterates over each
instruction, converts it to a 16-bit Hack machine instruction,
and writes it out in binary form.
*/

void assemble(const char *file_name, instruction *instructions, int num_instructions) {
    char out_name[300];
    snprintf(out_name, sizeof(out_name), "%s.hack", file_name);

    FILE *out = fopen(out_name, "wb");
    if (out == NULL) {
        printf("ERROR: Could not create output file %s\n", out_name);
        return;
    }

    int16_t next_var_addr = 16;

    for (int i = 0; i < num_instructions; i++) {
        instruction instr = instructions[i];
        opcode op = 0;

        if (instr.type == A_INSTR) {
            if (instr.a.is_addr) {
                op = instr.a.address;
            } else {
                Symbol *sym = symtable_find(instr.a.label);

                if (sym != NULL) {
                    op = sym->addr;
                } else {
                    symtable_insert(instr.a.label, next_var_addr);
                    op = next_var_addr;
                    next_var_addr++;
                }

                free(instr.a.label);
            }
        } else if (instr.type == C_INSTR) {

            op = instruction_to_opcode(instr.c);
        }

        fprintf(out, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", OPCODE_TO_BINARY(op));

        if (i < num_instructions - 1)
            fprintf(out, "\n");
    }

    fclose(out);
}