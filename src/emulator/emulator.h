#ifndef ARM11_35_EMULATOR_H
#define ARM11_35_EMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/arm11.h"

extern const int PC;
extern const int CPSR;

typedef struct Emulator {
    uint8_t *memory;
    int32_t *registers;
    bool execute_flag;
    bool decode_flag;
    bool halt_flag;
    uint32_t fetched;
    Instruction decoded;
} Emulator;

uint32_t get_memory(Emulator *emulator, uint32_t index);

void set_memory(Emulator *emulator, uint32_t index, uint32_t value);

Emulator *initialize_emulator(void);

void free_emulator(Emulator *emulator);

#endif //ARM11_35_EMULATOR_H
