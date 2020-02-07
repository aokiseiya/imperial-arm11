#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble.h"
#include "assembler.h"
#include "struct_converter.h"
#include "binary_converter.h"

int main(int argc, char **argv) {
  // First, we will load the assembly file into an array of strings.
  // Here we will assume that each line has at most 32 characters.

  //Open and read file
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "File open error");
    exit(EXIT_FAILURE);
  }

  // Since each line will represent 4 bytes and the maximum size is 64KB,
  // the maximum number of lines is 32768 (assuming every line has a label).
  AssemblyInstructions *assembly_code = calloc(INSTRUCTION_COUNT * 2, sizeof(AssemblyInstruction));
  if (assembly_code == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  // This value will keep track of the number of instructions.
  int size;

  // We will load the assembly code into a buffer of maximum size first.
  load_from_file(fp, assembly_code, &size);

  // Initialize Assembler struct which will carry information required across
  // all instructions.
  Assembler *assembler = initialize_assembler();

  // Pass one will make sure space is used efficiently and take the
  // instructions from the buffer and put them in the heap of a minimum
  // required size.
  // Note that pass_one will modify the size variable as required.
  SplitInstructions *split_instructions = pass_one(assembly_code, &size, assembler->label_st);

  // This will free the buffer that was used to load the file content.
  free(assembly_code);

  pass_two(split_instructions, &size, argv[2], assembler);

  // We will now free everything on the heap.
  free(split_instructions);

  free_assembler(assembler);

  return EXIT_SUCCESS;
}

SplitInstructions *pass_one(AssemblyInstructions *assembly_instructions, int *size, HashTable *label_st) {
  SplitInstructions *split_instructions = calloc(INSTRUCTION_COUNT, sizeof(SplitString *));
  if (split_instructions == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }
  // Split assembly code into array of instruction strings.

  int initial_size = *size;
  // j represents the address location of the current instruction.
  int j = 0;
  for (int i = 0; i < initial_size; i++) {
    char *word = (*assembly_instructions)[i];
    // Assuming input is syntactically correct, all labels will end with ':'.
    // We now test if the input is a label and process it accordingly.
    if (word[strlen(word) - 1] == ':') {
      int address = (j);
      word[strlen(word) - 1] = '\0';
      insert_si(label_st, word, address);
      --(*size);
    } else {
      (*split_instructions)[j] = split_string(word);
      ++j;
    }
  }

  // We will now reallocate the space to reduce redundant memory usage.
  // We know the final size we can set the size accordingly.
  SplitInstructions *new_split_instructions = realloc(split_instructions, (*size) * sizeof(SplitString *));
  if (new_split_instructions == NULL) {
    fprintf(stderr, "Reallocation failed");
    exit(EXIT_FAILURE);
  }

  split_instructions = new_split_instructions;
  return split_instructions;
}

void pass_two(SplitInstructions *split_instructions, const int *size, char *file_name, Assembler *assembler) {
  // We will calculate and store the memory location of where constants
  // (that are too large to be placed in the instructions) should be stored.
  // Notice that an offset of 1 is added to consider for halting instruction.
  assembler->const_index = (*size);

  // We will create a buffer for the binary output before we write it
  // to the output file.
  assembler->binary_output = calloc(((*size) * 2) + 1, sizeof(uint32_t));
  if (assembler->binary_output == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  for (assembler->current_index = 0; assembler->current_index < (*size); ++assembler->current_index) {
    // We declare an Instruction struct. We do not need to initialize it ass the contents will be overwritten anyways.
    Instruction instruction;
    // Pass into the struct converter.
    to_struct((*split_instructions)[assembler->current_index], &instruction, assembler);
    free_split_string((*split_instructions)[assembler->current_index]);
    assembler->binary_output[assembler->current_index] = to_binary(&instruction);
  }

  // Now we will write to binary file. To do this, we first open the file stream.
  FILE *out = fopen(file_name, "wb");

  // The const_index always points to the address after the last instruction.
  // This is the equivalent to pointing to last index + 1, which is the same
  // as the intended number of instructions to be written.
  fwrite(assembler->binary_output, sizeof(uint32_t), assembler->const_index, out);

  fclose(out);
}

// Loads file content to array for 32-char strings.
void load_from_file(FILE *fp, AssemblyInstructions *assembly_instructions, int *size) {
  // The size will also act as a counter
  *size = 0;
  while (true) {
    char next[sizeof(AssemblyInstruction) / sizeof(char)] = {0};

    // The loop can terminate
    if (fgets(next, sizeof(AssemblyInstruction) / sizeof(char), fp) == NULL) {
      break;
    }

    // If first element is a null terminator,
    // then we can assume whole string is null and

    // This branch will make sure empty lines are skipped.
    // Note if strcmp returns a zero, next == "\n".
    if (strcmp(next, "\n") != 0) {
      // The instruction line has a '\n' at the end.

      // This will be removed to simplify later operations.
      // We will use strtok for this;
      strtok(next, "\n");
      strncpy((*assembly_instructions)[*size], next, strlen(next));
      ++(*size);
    }
  }
  fclose(fp);
}

