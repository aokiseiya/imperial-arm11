#ifndef ARM11_35_ASSEMBLER_H
#define ARM11_35_ASSEMBLER_H

#include <stdint.h>
#include "../utils/hash_table.h"

// Some macros for global variables that are used throughout the assembler.
#define INSTRUCTION_SIZE 511
#define INSTRUCTION_COUNT 32768

typedef struct {
    int current_index;
    int const_index;
    uint32_t *binary_output;
    HashTable *label_st;
    HashTable *instruction_type_table;
    HashTable *cond_table;
    HashTable *opcode_table;
    HashTable *shift_table;
} Assembler;

Assembler *initialize_assembler(void);

void free_assembler(Assembler *assembler);

#endif //ARM11_35_ASSEMBLER_H
