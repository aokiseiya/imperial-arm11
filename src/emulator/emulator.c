#include <stdint.h>
#include <stdbool.h>
#include "emulator.h"

const int PC = 15;
const int CPSR = 16;

uint32_t get_memory(Emulator *emulator, uint32_t index) {
  uint32_t output = 0;
  for (int i = 0; i < 4; ++i) {
    output |= (emulator->memory[index + i] << 8 * i);
  }
  return output;
}

void set_memory(Emulator *emulator, uint32_t index, uint32_t value) {
  for (int i = 0; i < 4; ++i) {
    emulator->memory[index + i] = filter_bits(value, 8, i * 8);
  }
}

// Creates and initializes emulator.
Emulator *initialize_emulator(void) {
  Emulator *emulator = malloc(sizeof(Emulator));

  // We will initialize the struct
  emulator->decode_flag = false;
  emulator->execute_flag = false;
  emulator->halt_flag = false;

  // Model the machine's 64KB memory as a array of uint32_t
  emulator->memory = calloc(65536, sizeof(uint8_t));
  if (emulator->memory == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  emulator->registers = calloc(17, sizeof(uint32_t));
  if (emulator->registers == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  return emulator;
}

void free_emulator(Emulator *emulator) {
  free(emulator->registers);
  free(emulator->memory);
  free(emulator);
}