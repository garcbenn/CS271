/****************************************
 * C-ploration 10 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/
#ifndef __HACK_H__
#define __HACK_H__

#include <string.h>
#include <stdlib.h>


#define NUM_PREDEFINED_SYMBOLS 23

#define OPCODE_TO_BINARY(opcode) \
    ((opcode & 0x8000) ? '1' : '0'), \
    ((opcode & 0x4000) ? '1' : '0'), \
    ((opcode & 0x2000) ? '1' : '0'), \
    ((opcode & 0x1000) ? '1' : '0'), \
    ((opcode & 0x0800) ? '1' : '0'), \
    ((opcode & 0x0400) ? '1' : '0'), \
    ((opcode & 0x0200) ? '1' : '0'), \
    ((opcode & 0x0100) ? '1' : '0'), \
    ((opcode & 0x0080) ? '1' : '0'), \
    ((opcode & 0x0040) ? '1' : '0'), \
    ((opcode & 0x0020) ? '1' : '0'), \
    ((opcode & 0x0010) ? '1' : '0'), \
    ((opcode & 0x0008) ? '1' : '0'), \
    ((opcode & 0x0004) ? '1' : '0'), \
    ((opcode & 0x0002) ? '1' : '0'), \
    ((opcode & 0x0001) ? '1' : '0')

typedef enum symbol_id {
	SYM_SP = 0,
	SYM_LCL = 1,
	SYM_ARG = 2,
	SYM_THIS = 3,
	SYM_THAT = 4,

	SYM_R0 = 0,
    SYM_R1 = 1,
    SYM_R2 = 2,
    SYM_R3 = 3,
    SYM_R4 = 4,
    SYM_R5 = 5,
    SYM_R6 = 6,
    SYM_R7 = 7,
    SYM_R8 = 8,
    SYM_R9 = 9,
    SYM_R10 = 10,
    SYM_R11 = 11,
    SYM_R12 = 12,
    SYM_R13 = 13,
    SYM_R14 = 14,
    SYM_R15 = 15,

    SYM_SCREEN = 16384,
    SYM_KBD = 24576
} symbol_id;

typedef struct predefined_symbol {
	char name[50+1];
	int16_t address;
} predefined_symbol;


static const predefined_symbol predefined_symbols[NUM_PREDEFINED_SYMBOLS] = {
    {"SP", SYM_SP},
    {"LCL", SYM_LCL},
    {"ARG", SYM_ARG},
    {"THIS", SYM_THIS},
    {"THAT", SYM_THAT},

    {"R0", SYM_R0},
    {"R1", SYM_R1},
    {"R2", SYM_R2},
    {"R3", SYM_R3},
    {"R4", SYM_R4},
    {"R5", SYM_R5},
    {"R6", SYM_R6},
    {"R7", SYM_R7},
    {"R8", SYM_R8},
    {"R9", SYM_R9},
    {"R10", SYM_R10},
    {"R11", SYM_R11},
    {"R12", SYM_R12},
    {"R13", SYM_R13},
    {"R14", SYM_R14},
    {"R15", SYM_R15},

    {"SCREEN", SYM_SCREEN},
    {"KBD", SYM_KBD}
};


typedef enum jump_id {
    JMP_INVALID = -1,
    JMP_NULL = 0,

    JMP_JGT = 1,
    JMP_JEQ = 2,
    JMP_JGE = 3,
    JMP_JLT = 4,
    JMP_JNE = 5,
    JMP_JLE = 6,
    JMP_JMP = 7
} jump_id;


static inline jump_id str_to_jumpid(const char *s){
    jump_id id = JMP_INVALID;

    if (s == NULL) {
        return JMP_NULL;
    } else if (!strcmp(s, "JGT")) {
        id = JMP_JGT;
    } else if (!strcmp(s, "JEQ")) {
        id = JMP_JEQ;
    } else if (!strcmp(s, "JGE")) {
        id = JMP_JGE;  
    } else if (!strcmp(s, "JLT")) {
        id = JMP_JLT;
    } else if (!strcmp(s, "JNE")) {
        id = JMP_JNE;
    } else if (!strcmp(s, "JLE")) {
        id = JMP_JLE;
    } else if (!strcmp(s, "JMP")) {
        id = JMP_JMP;
    }

    return id;
}


typedef enum dest_id {
    DEST_INVALID = -1,
    DEST_NULL = 0,

    DEST_M = 1,
    DEST_D = 2,
    DEST_MD = 3,
    DEST_A = 4,
    DEST_AM = 5,
    DEST_AD = 6,
    DEST_AMD = 7
} dest_id;

static inline dest_id str_to_destid(const char *s) {
    dest_id id = DEST_INVALID;

    if (s == NULL) {
        id = DEST_NULL;
    } else if (!strcmp(s, "M")) {
        id = DEST_M;
    } else if (!strcmp(s, "D")) {
        id = DEST_D;
    } else if (!strcmp(s, "MD")) {
        id = DEST_MD;
    } else if (!strcmp(s, "A")) {
        id = DEST_A;
    } else if (!strcmp(s, "AM")) {
        id = DEST_AM;
    } else if (!strcmp(s, "AD")) {
        id = DEST_AD;
    } else if (!strcmp(s, "AMD")) {
        id = DEST_AMD;
    }

    return id;
}

typedef enum comp_id {
    COMP_INVALID = -1,

    COMP_0 = 0b0101010,
    COMP_1 = 0b0111111,
    COMP_NEG1 = 0b0111010,
    COMP_D = 0b0001100,
    COMP_A = 0b0110000,
    COMP_NOTD = 0b0001101,
    COMP_NOTA = 0b0110001,
    COMP_NEGD = 0b0001111,
    COMP_NEGA = 0b0110011,
    COMP_DPLUS1 = 0b0011111,
    COMP_APLUS1 = 0b0110111,
    COMP_DMINUS1 = 0b0001110,
    COMP_AMINUS1 = 0b0110010,
    COMP_DPLUSA = 0b0000010,
    COMP_DMINUSA = 0b0010011,
    COMP_AMINUSD = 0b0000111,
    COMP_DANDA = 0b0000000,
    COMP_DORA = 0b0010101,

    COMP_M = 0b1110000,
    COMP_NOTM = 0b1110001,
    COMP_NEGM = 0b1110011,
    COMP_MPLUS1 = 0b1110111,
    COMP_MMINUS1 = 0b1110010,
    COMP_DPLUSM = 0b1000010,
    COMP_DMINUSM = 0b1010011,
    COMP_MMINUSD = 0b1000111,
    COMP_DANDM = 0b1000000,
    COMP_DORM = 0b1010101
} comp_id;

static inline comp_id str_to_compid(const char *s, int *a) {
    comp_id id = COMP_INVALID;

    if (s == NULL) {
        return id;
    }

    if (!strcmp(s, "0")) {
        *a = 0; 
        id = COMP_0;
    } else if (!strcmp(s, "1")) {
        *a = 0;
        id = COMP_1;
    } else if (!strcmp(s, "-1")) {
        *a = 0;
        id = COMP_NEG1;
    } else if (!strcmp(s, "D")) {
        *a = 0;
        id = COMP_D;
    } else if (!strcmp(s, "A")) {
        *a = 0;
        id = COMP_A;
    } else if (!strcmp(s, "!D")) {
        *a = 0;
        id = COMP_NOTD;
    } else if (!strcmp(s, "!A")) {
        *a = 0;
        id = COMP_NOTA;
    } else if (!strcmp(s, "-D")) {
        *a = 0;
        id = COMP_NEGD;
    } else if (!strcmp(s, "-A")) {
        *a = 0;
        id = COMP_NEGA;
    } else if (!strcmp(s, "D+1")) {
        *a = 0;
        id = COMP_DPLUS1;
    } else if (!strcmp(s, "A+1")) {
        *a = 0;
        id = COMP_APLUS1;
    } else if (!strcmp(s, "D-1")) {
        *a = 0;
        id = COMP_DMINUS1;
    } else if (!strcmp(s, "A-1")) {
        *a = 0;
        id = COMP_AMINUS1;
    } else if (!strcmp(s, "D+A")) {
        *a = 0;
        id = COMP_DPLUSA;
    } else if (!strcmp(s, "D-A")) {
        *a = 0;
        id = COMP_DMINUSA;
    } else if (!strcmp(s, "A-D")) {
        *a = 0;
        id = COMP_AMINUSD;
    } else if (!strcmp(s, "D&A")) {
        *a = 0;
        id = COMP_DANDA;
    } else if (!strcmp(s, "D|A")) {
        *a = 0;
        id = COMP_DORA;
    } else if (!strcmp(s, "M")) {
        *a = 1;
        id = COMP_M;
    } else if (!strcmp(s, "!M")) {
        *a = 1;
        id = COMP_NOTM;
    } else if (!strcmp(s, "-M")) {
        *a = 1;
        id = COMP_NEGM;
    } else if (!strcmp(s, "M+1")) {
        *a = 1;
        id = COMP_MPLUS1;
    } else if (!strcmp(s, "M-1")) {
        *a = 1;
        id = COMP_MMINUS1;
    } else if (!strcmp(s, "D+M")) {
        *a = 1;
        id = COMP_DPLUSM;
    } else if (!strcmp(s, "D-M")) {
        *a = 1;
        id = COMP_DMINUSM;
    } else if (!strcmp(s, "M-D")) {
        *a = 1;
        id = COMP_MMINUSD;
    } else if (!strcmp(s, "D&M")) {
        *a = 1;
        id = COMP_DANDM;
    } else if (!strcmp(s, "D|M")) {
        *a = 1;
        id = COMP_DORM;
    } 

    return id;
}

#endif