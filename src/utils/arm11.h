#ifndef ARM11_35_ARM11_H
#define ARM11_35_ARM11_H

#include <stdint.h>
#include <stdbool.h>

typedef enum Inst_Type {
    DATAP = 0x0,
    MULTIPLY = 0x3,
    SINGLEDT = 0x1,
    BRANCH = 0x2
} Inst_Type;

typedef enum Cond {
    EQ = 0x0,
    NE = 0x1,
    GE = 0xA,
    LT = 0xB,
    GT = 0xC,
    LE = 0xD,
    AL = 0xE
} Cond;

typedef enum OpCode {
    AND = 0x0,
    EOR = 0x1,
    SUB = 0x2,
    RSB = 0x3,
    ADD = 0x4,
    TST = 0x8,
    TEQ = 0x9,
    CMP = 0xA,
    ORR = 0xC,
    MOV = 0xD,
    SPECIAL_LSL = 0xE,
    SPECIAL_ANDEQ = 0xF
} OpCode;

typedef enum Shift_Type {
    LSL = 0x0,
    LSR = 0x1,
    ASR = 0x2,
    ROR = 0x3
} Shift_Type;

typedef struct Instruction {
    enum Inst_Type inst_type;
    enum Cond cond;
    enum OpCode opcode;
    uint32_t rn;
    uint32_t rd;
    uint32_t rs;
    uint32_t rm;
    uint32_t offset;
    uint32_t operand;
    bool i;
    bool a;
    bool s;
    bool l;
    bool p;
    bool u;
    bool halt;
} Instruction;

inline uint32_t mask(uint32_t width) {
  return (1 << width) - 1;
}

inline uint32_t filter_bits(uint32_t input, uint32_t width, uint32_t shift) {
  return (input >> shift) & mask(width);
}

inline uint32_t set_bits(uint32_t input, uint32_t width, uint32_t shift) {
  return (input) | (mask(width) << shift);
}

#endif //ARM11_35_ARM11_H