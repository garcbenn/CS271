/****************************************
 * C-ploration 5 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH (MAX_LINE_LENGTH - 2)

/** function prototypes **/
char *strip(char *s);

void parse(FILE *file);

bool is_Atype (const char *);
bool is_label (const char *);
bool is_Ctype (const char *);

typedef int16_t hack_addr;
typedef int16_t opcode;

typedef enum instr_type {
	INVALID_INSTR = -1,
	A_INSTR,
	C_INSTR
} instr_type;

typedef struct c_instruction {
	opcode a : 1;
	opcode comp : 7;
	opcode dest : 3;
	opcode jump : 3;
} c_instruction;

typedef struct a_instruction {
	union {
		hack_addr address;
		char *label;
	};
	bool is_addr;
} a_instruction;

typedef struct instruction {
	union {
		a_instruction a;
		c_instruction c;
	};
	instr_type type;
} instruction;

char *extract_label(const char *line, char *label);

#endif