README – CS 271 Project 6: Hack Assembler
Overview

This project implements a two-pass assembler for the Hack computer platform described in The Elements of Computing Systems. The assembler reads a .asm file, parses its A- and C-instructions, resolves symbols, and outputs the corresponding 16-bit machine code in a .hack file. The implementation builds on C-plorations 1–10.

Driver Program

The driver program is main.c. It expects exactly one command-line argument: the path to a .asm file. It does not prompt the user.

Example:

./main.exe path/to/Program.asm


The assembler produces:

path/to/Program.asm.hack

Building the Assembler

A standard Makefile is provided.

To build:

make


To clean:

make clean

How the Assembler Works

The assembler performs two passes over the input file:

Pass 1 (Parsing and Symbol Collection)

Strips whitespace and comments

Identifies label declarations (LABEL)

Inserts labels into the symbol table with their ROM addresses

Produces an array of parsed instructions

Pass 2 (Translation)

Resolves A-instruction addresses (numeric or symbolic)

Allocates new variables starting at RAM address 16

Converts C-instructions into 16-bit opcodes using comp/dest/jump tables

Writes one 16-bit machine instruction per line to the output file

Testing

The assembler was tested using the official nand2tetris tools on the following programs:

Add.asm

Max.asm / MaxL.asm

Rect.asm / RectL.asm

Pong.asm / PongL.asm

All generated .hack files match the reference outputs provided by the nand2tetris assembler.

Error Handling

The program checks for:

Missing or incorrect command-line arguments

Invalid input filenames

Invalid A- and C-instruction formats

Duplicate or malformed labels

Excess instructions beyond the allowed memory size

Errors are reported using the provided exit_program mechanism.

File Structure
main.c
parser.c
parser.h
hack.h
symtable.c
symtable.h
error.c
error.h
Makefile
README.md
