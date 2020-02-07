#include "decode.h"
#include "emulator.h"

// The reason why this declaration is made is to make sure that this file is
// not recompiled for every unique file that uses the function.
// Although this is not necessary as fetch is only called from emulate, this has
// been done for consistency.

extern inline void set_instruction_all(Instruction *instruction, uint32_t binary_inst);

extern inline void decode(Emulator *emulator);