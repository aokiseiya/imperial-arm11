#ifndef ARM11_35_STRUCT_CONVERTER_H
#define ARM11_35_STRUCT_CONVERTER_H

#include <stdint.h>
#include "../utils/arm11.h"
#include "../utils/split_string.h"
#include "../utils/hash_table.h"
#include "assembler.h"

char *remove_brackets(char *s);

void to_struct(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

// Helper functions for to_struct
void to_struct_data_processing(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_single_data_transfer(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_branch(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_multiply(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

// Helper functions for to_struct_data_processing
void to_struct_compute(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_no_compute(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_mov(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_lsl(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_andeq(SplitString *split_strings, Instruction *instruction, Assembler *assembler);

void to_struct_error(SplitString *split_strings, Instruction *instruction, Assembler *assembler);


#endif //ARM11_35_STRUCT_CONVERTER_H
