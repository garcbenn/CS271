/****************************************
 * C-ploration 9 for CS 271
 * 
 * [NAME] Benny Garcia
 * [TERM] FALL 2025
 * 
 ****************************************/

#include "symtable.h"

Symbol* hashArray[SYMBOL_TABLE_SIZE];


int hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % SYMBOL_TABLE_SIZE;
}

void symtable_insert(char *key, hack_addr addr) {
    int index = hash(key);

    // Allocate a new Symbol struct
    Symbol *item = (Symbol*) malloc(sizeof(Symbol));
    item->name = strdup(key);   // duplicate the key string
    item->addr = addr;

    // Linear probing to resolve collisions
    while (hashArray[index] != NULL) {
        index = (index + 1) % SYMBOL_TABLE_SIZE;
    }

    hashArray[index] = item;
}

Symbol *symtable_find(char *key) {
    int index = hash(key);

    while (hashArray[index] != NULL) {
        // Check if the stored key matches the search key
        if (strcmp(hashArray[index]->name, key) == 0) {
            return hashArray[index];
        }

        // Move to next slot
        index = (index + 1) % SYMBOL_TABLE_SIZE;
    }

    return NULL;  // Not found
}

void symtable_display_table() {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        if (hashArray[i] != NULL) {
            printf("Index %d: {%s, %d}\n",
                   i,
                   hashArray[i]->name,
                   hashArray[i]->addr);
        }
    }
}

void symtable_print_labels() {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        if (hashArray[i] != NULL) {
            printf("{%s,%d}\n",
                   hashArray[i]->name,
                   hashArray[i]->addr);
        }
    }
}