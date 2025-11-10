/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){	

	char s_new[strlen(s) +1];
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

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE * file){
	
	char line[MAX_LINE_LENGTH];

	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, "\r\n")] = 0;
		strip(line);
		if (!*line) continue;

		char inst_type;
		if (is_Atype(line)) {
			inst_type = 'A';
		} else if (is_label(line)) {
			inst_type = 'L';
	
    char label[MAX_LABEL_LENGTH];
    	extract_label(line, label);
    	strcpy(line, label);
	} else if (is_Ctype(line)) {
		inst_type = 'C';
	}
		
		printf("%c  %s\n", inst_type, line);
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