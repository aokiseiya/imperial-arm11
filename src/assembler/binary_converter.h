#ifndef ARM11_35_BINARY_CONVERTER_H
#define ARM11_35_BINARY_CONVERTER_H

#include "../utils/arm11.h"

static inline uint32_t to_binary_cond(Instruction *instruction) {
  return instruction->cond;
}

static inline uint32_t to_binary_opcode(Instruction *instruction) {
  return instruction->opcode;
}

static inline uint32_t to_binary_operand(Instruction *instruction) {
  return instruction->operand;
}

static inline uint32_t to_binary_offset(Instruction *instruction) {
  return instruction->offset;
}

static inline uint32_t to_binary_rn(Instruction *instruction) {
  return instruction->rn;
}

static inline uint32_t to_binary_rd(Instruction *instruction) {
  return instruction->rd;
}

static inline uint32_t to_binary_rs(Instruction *instruction) {
  return instruction->rs;
}

static inline uint32_t to_binary_rm(Instruction *instruction) {
  return instruction->rm;
}

static inline uint32_t to_binary_i(Instruction *instruction) {
  return (instruction->i != false) ? 1 : 0;
}

static inline uint32_t to_binary_s(Instruction *instruction) {
  return (instruction->s != false) ? 1 : 0;
}

static inline uint32_t to_binary_l(Instruction *instruction) {
  return (instruction->l != false) ? 1 : 0;
}

static inline uint32_t to_binary_a(Instruction *instruction) {
  return (instruction->a != false) ? 1 : 0;
}

static inline uint32_t to_binary_p(Instruction *instruction) {
  return (instruction->p != false) ? 1 : 0;
}

static inline uint32_t to_binary_u(Instruction *instruction) {
  return (instruction->u != false) ? 1 : 0;
}

uint32_t to_binary(Instruction *instruction);

#endif //ARM11_35_BINARY_CONVERTER_H
