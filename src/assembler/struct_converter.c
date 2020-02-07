#include "struct_converter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"
#include "../utils/arm11.h"

#include "../utils/hash_table.h"

uint32_t shift_to_int(char **shift, Instruction *instruction, Assembler *assembler);

uint32_t calculate_operand(char **operand, Instruction *instruction, bool shift_flag, Assembler *assembler);

void set_operand(char **operand, Instruction *instruction, bool shift_flag, Assembler *assembler);

void set_offset(char **operand, Instruction *instruction, bool shift_flag, Assembler *assembler);

char *remove_brackets(char *s) {
  s[strlen(s) - 1] = '\0';
  return &s[1];
}

// Function pointer for to_struct_data_processing
// Points to appropriate function according to opCode of the instruction
void (*opcode_ptr[SPECIAL_ANDEQ + 1 ])(SplitString *, Instruction *, Assembler *) =
        {to_struct_compute, to_struct_compute, to_struct_compute, to_struct_compute,
         to_struct_compute, to_struct_error, to_struct_error, to_struct_error,
         to_struct_no_compute, to_struct_no_compute, to_struct_no_compute, to_struct_error,
         to_struct_compute, to_struct_mov, to_struct_lsl, to_struct_andeq};

// Function pointer for to_struct
// Points to appropriate function according to inst_type
void (*struct_ptr[4])(SplitString *, Instruction *, Assembler *) =
        {to_struct_data_processing, to_struct_single_data_transfer, to_struct_branch, to_struct_multiply};


// Helper function for set_operand and set_offset
uint32_t shift_to_int(char **shift, Instruction *instruction, Assembler *assembler) {
  instruction->rm = strtol(&shift[0][1], NULL, 10);

  // Identify shift type
  uint32_t shiftType = search_si(assembler->shift_table, shift[1]);
  uint32_t shiftImm = (shiftType << 1);

  if (shift[2][0] == 'r') {
    // Shift specified by a register
    instruction->rs = strtol(&shift[2][1], NULL, 10);
    shiftImm |= (instruction->rs << 4) + 1;
  } else {
    // Shift by a constant amount
    shiftImm |= (strtol(&shift[2][1], NULL, 0) << 3);
  }

  return (shiftImm << 4) + instruction->rm;
}

uint32_t calculate_operand(char **operand, Instruction *instruction, bool shift_flag, Assembler *assembler) {
  if (operand[0][0] == '#') {
    // Operand is an immediate value in the form of <#expression>
    // The immediate is rotated to fit in the form
    uint32_t immediate = strtol(&operand[0][1], NULL, 0);
    uint32_t rotate = 0;

    while (immediate > 0xFF) {
      // The immediate is too big to be represented
      if (rotate > 0xF) {
        fprintf(stderr, "The numeric constant cannot be represented");
        exit(EXIT_FAILURE);
      }
      // Rotates left by 2 and increments rotate field by 1
      immediate = (immediate << 2) | (immediate >> 30);
      rotate++;
    }

    return (rotate << 8) + immediate;
  } else {
    // If shift_flag is set, operand is a shifted register in the form of Rm{,<shift>}
    // Otherwise it is a non-shifted register
    return (shift_flag) ? shift_to_int(operand, instruction, assembler) : strtol(&operand[0][1], NULL, 10);
  }
}

// Sets operand of data processing instruction
void set_operand(char **operand, Instruction *instruction, bool shift_flag, Assembler *assembler) {
  instruction->operand = calculate_operand(operand, instruction, shift_flag, assembler);
  instruction->i = (operand[0][0] == '#') ? 1 : 0;
}

// Sets offset of single data transfer instruction
void set_offset(char **operand, Instruction *instruction, bool shift_flag, Assembler *assembler) {
  instruction->offset = calculate_operand(operand, instruction, shift_flag, assembler);
  instruction->i = (operand[0][0] == '#') ? 0 : 1;
}

void to_struct(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  // Executes appropriate function according to inst_type
  instruction->inst_type = search_si(assembler->instruction_type_table, split_strings->words[0]);
  instruction->cond = search_si(assembler->cond_table, split_strings->words[0]);
  struct_ptr[instruction->inst_type](split_strings, instruction, assembler);
}

void to_struct_data_processing(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  // Executes appropriate function according to opCode
  instruction->opcode = search_si(assembler->opcode_table, split_strings->words[0]);
  opcode_ptr[instruction->opcode](split_strings, instruction, assembler);
}

void to_struct_single_data_transfer(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  instruction->i = 0;
  instruction->u = 1;
  instruction->offset = 0;
  instruction->rd = strtol(&split_strings->words[1][1], NULL, 10);
  // Loading
  instruction->l = strcmp(split_strings->words[0], "ldr") ? 0 : 1;

  if (split_strings->words[2][0] == '=') {
    // Address is a numeric constant of the form: <=expression>
    char new_inst[INSTRUCTION_SIZE];

    if (strtol(&split_strings->words[2][1], NULL, 16) <= 0xFF) {
      // If value of expression fits in the argument of a mov, change to mov instruction
      split_strings->words[2][0] = '#';
      sprintf(new_inst, "mov %s, %s", split_strings->words[1], split_strings->words[2]);
    } else {
      // Otherwise changed to ldr instruction with shifted PC register (r15)
      // and append register containing address of expression to end
      assembler->binary_output[(int) (assembler->const_index)] = strtol(&split_strings->words[2][1], NULL, 16);
      uint32_t offset = (assembler->const_index - assembler->current_index - 2) << 2;
      ++(assembler->const_index);
      sprintf(new_inst, "ldr %s,[r15,#%u]", split_strings->words[1], offset);
    }

    SplitString *new_split_string = split_string(new_inst);
    to_struct(new_split_string, instruction, assembler);
    free_split_string(new_split_string);
  } else {
    // Initialise offset and determine between pre and post-indexing
    SplitString *offset = split_string(remove_brackets(split_strings->words[2]));

    if (split_strings->size == 3) {
      // Pre-indexing, set flag
      instruction->p = 1;
    } else {
      // Post-indexing
      instruction->p = 0;
      char new_offset[INSTRUCTION_SIZE];
      sprintf(new_offset, "%s, %s", offset->words[0], split_strings->words[3]);
      offset = split_string(new_offset);
    }

    instruction->rn = strtol(&offset->words[0][1], NULL, 10);

    if (offset->size != 1) {
      // The address is in the form of [Rn, <#expression>] or [Rn, {+/-}Rm{, <shift>}]
      if (offset->words[1][1] == '-') {
        // Remove minus sign and set a sign flag
        offset->words[1] = &offset->words[1][1];
        offset->words[1][0] = '#';
        instruction->u = 0;
      }
      set_offset(&offset->words[1], instruction, offset->size != 2, assembler);
    }
    free_split_string(offset);
  }
}

void to_struct_branch(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  // We will assume that all branches are to labels.
  // Calculates current address and target address
  uint32_t target_address = search_si(assembler->label_st, split_strings->words[1]);

  // int32_t uses two's complement already. We will truncate it to 24 bits manually.
  int32_t offset = (target_address - assembler->current_index) - 2;
  instruction->offset = offset & mask(24);
}

void to_struct_multiply(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  instruction->s = 0;
  instruction->a = 0;
  instruction->rd = strtol(&split_strings->words[1][1], NULL, 10);
  instruction->rm = strtol(&split_strings->words[2][1], NULL, 10);
  instruction->rs = strtol(&split_strings->words[3][1], NULL, 10);

  if (!strcmp(split_strings->words[0], "mla")) {
    instruction->a = 1;
    instruction->rn = strtol(&split_strings->words[4][1], NULL, 10);
  }
}

void to_struct_compute(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  // When the instruction is AND, EOR, SUB, RSB, ADD, ORR
  instruction->s = 0;
  instruction->rn = strtol(&split_strings->words[2][1], NULL, 10);
  instruction->rd = strtol(&split_strings->words[1][1], NULL, 10);
  set_operand(&split_strings->words[3], instruction, split_strings->size == 6, assembler);
}

void to_struct_no_compute(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  // When the instruction is TST, TEQ or CMP
  instruction->s = 1;
  instruction->rn = strtol(&split_strings->words[1][1], NULL, 10);
  instruction->rd = 0;
  set_operand(&split_strings->words[2], instruction, split_strings->size == 5, assembler);
}

void to_struct_mov(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  // When the instruction is MOV
  instruction->s = 0;
  instruction->rn = 0;
  instruction->rd = strtol(&split_strings->words[1][1], NULL, 10);
  set_operand(&split_strings->words[2], instruction, split_strings->size == 5, assembler);
}

void to_struct_lsl(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  char new_inst[INSTRUCTION_SIZE];
  sprintf(new_inst, "mov %s, %s, lsl %s",
          split_strings->words[1], split_strings->words[1], split_strings->words[2]);
  SplitString *new_split_string = split_string(new_inst);
  to_struct_data_processing(new_split_string, instruction, assembler);
  free_split_string(new_split_string);
}

void to_struct_andeq(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  instruction->cond = EQ;
  instruction->opcode = AND;
  to_struct_compute(split_strings, instruction, assembler);
}

// Generates an error message if OpCode is incorrect
void to_struct_error(SplitString *split_strings, Instruction *instruction, Assembler *assembler) {
  fprintf(stderr, "Unknown OpCode");
  exit(EXIT_FAILURE);
}
