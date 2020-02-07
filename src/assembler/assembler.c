#include "assembler.h"
#include <stdlib.h>

Assembler *initialize_assembler(void) {
  Assembler *assembler = calloc(1, sizeof(Assembler));
  assembler->label_st = create_hash_table();
  assembler->instruction_type_table = create_instruction_type_table();
  assembler->cond_table = create_cond_table();
  assembler->opcode_table = create_opcode_table();
  assembler->shift_table = create_shift_table();
  return assembler;
}

void free_assembler(Assembler *assembler) {
  free_hash_table(assembler->label_st);
  free_hash_table(assembler->instruction_type_table);
  free_hash_table(assembler->cond_table);
  free_hash_table(assembler->opcode_table);
  free_hash_table(assembler->shift_table);
  if (assembler->binary_output != NULL) {
    free(assembler->binary_output);
  }
  free(assembler);
}
