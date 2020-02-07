#include "emulate.h"
#include "emulator.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
  Emulator *emulator = initialize_emulator();

  //Open and read file as binary
  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    fprintf(stderr, "File open error");
    exit(EXIT_FAILURE);
  }


  // Load the binary file content into the emulator's memory
  if (fread(emulator->memory, sizeof(uint8_t), 65536, fp) == 0) {
    fprintf(stderr, "File is empty.");
    exit(EXIT_FAILURE);
  }

  // Now we can start the pipeline
  pipeline(emulator);

  print_output(emulator);

  free_emulator(emulator);

  return EXIT_SUCCESS;
}

static inline void pipeline(Emulator *emulator) {
  while (true) {
    if (emulator->execute_flag) {
      execute(emulator);
    }
    if (emulator->halt_flag) {
      return;
    }
    if (emulator->decode_flag) {
      decode(emulator);
    }
    // Must always fetch unless halt flag is set
    fetch(emulator);
  }
}

static inline void print_output(Emulator *emulator) {
  printf("Registers:\n");
  for (int i = 0; i < 17; i++) {
    int32_t contents = emulator->registers[i];
    switch (i) {
      case 13:
      case 14:
        break;
      case 15:
        printf("PC  : %10d (0x%08x)\n", contents, contents);
        break;
      case 16:
        printf("CPSR: %10d (0x%08x)\n", contents, contents);
        break;
      default:
        printf("$%-3d: %10d (0x%08x)\n", i, contents, contents);
        break;
    }
  }

  printf("Non-zero memory:\n");
  for (int i = 0; i < 65536; i += 4) {
    uint8_t content1 = emulator->memory[i];
    uint8_t content2 = emulator->memory[i + 1];
    uint8_t content3 = emulator->memory[i + 2];
    uint8_t content4 = emulator->memory[i + 3];

    if (content1 || content2 || content3 || content4) {
      printf("0x%08x: 0x%02x%02x%02x%02x\n",
             i, content1, content2, content3, content4);
    }
  }
}
