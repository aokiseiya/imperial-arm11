#ifndef ARM11_35_ASSEMBLE_H
#define ARM11_35_ASSEMBLE_H

#include "../utils/hash_table.h"
#include "../utils/split_string.h"
#include "assembler.h"

typedef char AssemblyInstruction[INSTRUCTION_SIZE];

typedef AssemblyInstruction AssemblyInstructions[INSTRUCTION_COUNT * 2];

typedef SplitString (*SplitInstructions[]);

SplitInstructions *pass_one(AssemblyInstructions *assembly_instructions, int *size, HashTable *label_st);

void pass_two(SplitInstructions *split_instructions, const int *size, char *file_name, Assembler *assembler);

void load_from_file(FILE *fp, AssemblyInstructions *assembly_instructions, int *size);


#endif //ARM11_35_ASSEMBLE_H