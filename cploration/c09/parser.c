/****************************************
 * C-ploration 9 for CS 271
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
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
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
 * -----------------------------
 * Parses an A-type instruction.
 *
 * line: the full A-instruction string
 * instr: output a_instruction struct to fill
 *
 * Returns true if valid, false if invalid label.
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

/*Function: add_predefined_symbols
 *-----------------------------
 *Loads the 23 predefined Hack symbols into the symbol table before parsing begins.
 *
 * This function iterates over the predefined_symbols[] array in hack.h
 * and inserts each symbol name/address pair into the symbol table.
 */
void add_predefined_symbols () {

    for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++) {
        predefined_symbol sym = predefined_symbols[i];
        symtable_insert(sym.name, sym.address);
    }
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE *file) {
    char line[MAX_LINE_LENGTH];
    unsigned int line_num = 0;
    unsigned int instr_num = 0;
    char inst_type;

    instruction instr;

    add_predefined_symbols();

    while (fgets(line, sizeof(line), file)) {
        line_num++;

        // Save original BEFORE modifications
        line[strcspn(line, "\r\n")] = '\0';

        // Strip whitespace & comments
        strip(line);

        // Skip blank lines
        if (line[0] == '\0') continue;

        // check max-instructions
        if (instr_num >= MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS);
        }

        // LABEL HANDLING
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
        }

        // Determine instruction type
        if (is_Atype(line)) {

            //Parse the A-instruction
            if (!parse_A_instruction(line, &instr.a)) {
                exit_program(EXIT_INVALID_A_INSTR, line_num, line);
            }

            instr.type = A_INSTR;

        } else {
            inst_type = 'C';
        }

        // Print instruction
        /*printf("%u: %c  %s\n", instr_num, inst_type, line);*/

        // Only increment for real instructions
        instr_num++;
    }
}


/* Function: extract_label
 * ------------------------
 * Takes a line like "(LABEL)" and copies "LABEL" into label.
 *
 * line: the input string (must be a label line)
 * label: output buffer to hold the label text
 *
 * returns: pointer to label
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