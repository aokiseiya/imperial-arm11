#ifndef ARM11_35_DECODE_H
#define ARM11_35_DECODE_H

#include "emulator.h"

inline void set_instruction_all(Instruction *instruction, uint32_t binary_inst) {
  instruction->inst_type = (filter_bits(binary_inst, 2, 26) != DATAP) ? filter_bits(binary_inst, 2, 26) :
                           ((filter_bits(binary_inst, 4, 4) == 0x9) ? MULTIPLY : DATAP);
  instruction->cond = (binary_inst & 0xF0000000) >> 28;
  instruction->opcode = (binary_inst & 0x01E00000) >> 21;
  instruction->rn = (binary_inst >> ((instruction->inst_type == MULTIPLY) ? 12 : 16)) & 0xF;
  instruction->rd = (binary_inst >> ((instruction->inst_type == MULTIPLY) ? 16 : 12)) & 0xF;
  instruction->rm = (binary_inst & 0x0000000F);
  instruction->rs = (binary_inst & 0x00000F00) >> 8;
  instruction->operand = binary_inst & 0x00000FFF;
  instruction->offset = binary_inst & ((instruction->inst_type == SINGLEDT) ? 0x00000FFF : 0x00FFFFFF);
  instruction->i = (binary_inst & 0x02000000) >> 25;
  instruction->a = (binary_inst & 0x00200000) >> 21;
  instruction->s = (binary_inst & 0x00100000) >> 20;
  instruction->l = (binary_inst & 0x00100000) >> 20;
  instruction->p = (binary_inst & 0x01000000) >> 24;
  instruction->u = (binary_inst & 0x00800000) >> 23;
  instruction->halt = binary_inst == 0;
}

inline void decode(Emulator *emulator) {
  set_instruction_all(&emulator->decoded, emulator->fetched);
  emulator->execute_flag = true;
}

#endif //ARM11_35_DECODE_H
