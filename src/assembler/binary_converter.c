#include "binary_converter.h"
#include <stdlib.h>
#include <stdio.h>

uint32_t to_binary(Instruction *instruction) {
  uint32_t cond = to_binary_cond(instruction);
  uint32_t opcode = to_binary_opcode(instruction);
  uint32_t operand = to_binary_operand(instruction);
  uint32_t offset = to_binary_offset(instruction);
  uint32_t rn = to_binary_rn(instruction);
  uint32_t rd = to_binary_rd(instruction);
  uint32_t rs = to_binary_rs(instruction);
  uint32_t rm = to_binary_rm(instruction);
  uint32_t i = to_binary_i(instruction);
  uint32_t s = to_binary_s(instruction);
  uint32_t l = to_binary_l(instruction);
  uint32_t a = to_binary_a(instruction);
  uint32_t p = to_binary_p(instruction);
  uint32_t u = to_binary_u(instruction);

  switch (instruction->inst_type) {
    case DATAP:
      return (cond << 28) | (i << 25) | (opcode << 21) | (s << 20) | (rn << 16) | (rd << 12) | operand;
    case MULTIPLY:
      return (cond << 28) | (a << 21) | (s << 20) | (rd << 16) | (rn << 12) | (rs << 8) | (0x9 << 4) | rm;
    case SINGLEDT:
      return (cond << 28) | (1 << 26) | (i << 25) | (p << 24) | (u << 23) | (l << 20) | (rn << 16) | (rd << 12) |
             offset;
    case BRANCH:
      return (cond << 28) | (0x5 << 25) | offset;
    default:
      fprintf(stderr, "Unknown instruction type");
      exit(EXIT_FAILURE);
  }
}