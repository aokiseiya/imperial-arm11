#include <stdio.h>
#include <stdlib.h>
#include "execute.h"

// Declare function pointer arrays for use.
static void (*exe_ptr[4])(Emulator *) =
        {execute_data_processing, execute_single_data_transfer, execute_branch, execute_multiply};

static uint32_t (*exe_shift_ptr[4])(uint32_t, uint32_t) =
        {logical_left, logical_right, arithmetic_right, rotate_right};

static uint32_t (*exe_opcode_ptr[MOV + 1])(uint32_t, uint32_t, uint8_t *) =
        {exe_and, exe_eor, exe_sub, exe_rsb, exe_add, exe_error, exe_error,
         exe_error, exe_and, exe_eor, exe_sub, exe_error, exe_orr, exe_mov};

void execute(Emulator *emulator) {
  if (emulator->decoded.halt) {
    emulator->halt_flag = true;
  } else if (check_cond(emulator)) {
    exe_ptr[emulator->decoded.inst_type](emulator);
  }
}

void execute_data_processing(Emulator *emulator) {
  Instruction *instruction = &emulator->decoded;
  uint8_t carry = 0;

  // Executes appropriate function according to OpCode
  uint32_t result = exe_opcode_ptr[instruction->opcode]
          (emulator->registers[instruction->rn], get_operand_datap(emulator, &carry), &carry);

  // Saves the result to destination
  if (instruction->opcode < TST || instruction->opcode > CMP) {
    emulator->registers[instruction->rd] = result;
  }

  if (instruction->s) {
    set_cpsr(emulator, result, carry);
  }
}

uint32_t get_operand_datap(Emulator *emulator, uint8_t *carry) {
  Instruction *instruction = &emulator->decoded;

  if (instruction->i) {
    // Operand2 is an immediate value
    return rotate_right(filter_bits(instruction->operand, 8, 0), filter_bits(instruction->operand, 4, 8) * 2);
  } else {
    // Operand2 is a register
    uint32_t rm = emulator->registers[instruction->rm];
    uint32_t shift;

    if (filter_bits(instruction->operand, 1, 4)) {
      // Shift specified by a register
      // The bottom byte of Rs specifies the amount to be shifted
      shift = filter_bits(emulator->registers[instruction->rs], 8, 0);
    } else {
      // Shift by a constant amount
      shift = filter_bits(instruction->operand, 5, 7);
    }

    // Returns the shifted value of rm
    if (shift) {
      uint32_t shift_type = filter_bits(instruction->operand, 2, 5);
      *carry = (shift_type == LSL) ? filter_bits(rm, 1, 32 - shift) : filter_bits(rm, 1, shift - 1);
      // Executes appropriate shift operation according to shift type
      return exe_shift_ptr[shift_type](rm, shift);
    }

    // Returns the value of rm
    return rm;
  }
}

uint32_t logical_left(uint32_t x, uint32_t n) {
  return (n >= 32) ? 0 : (x << n);
}

uint32_t logical_right(uint32_t x, uint32_t n) {
  return (x >> n);
}

uint32_t arithmetic_right(uint32_t x, uint32_t n) {
  return (filter_bits(x, 1, 31)) ? (x >> n) | mask(n) << (32 - n) : (x >> n);
}

uint32_t rotate_right(uint32_t x, uint32_t n) {
  return (x >> n) | (x << (32 - n));
}

uint32_t exe_and(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  return operand1 & operand2;
}

uint32_t exe_eor(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  return operand1 ^ operand2;
}

uint32_t exe_sub(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  *carry = operand1 >= operand2;
  return operand1 - operand2;
}

uint32_t exe_rsb(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  *carry = operand2 >= operand1;
  return operand2 - operand1;
}

uint32_t exe_add(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  uint32_t result = operand1 + operand2;
  *carry = operand1 > result;
  return result;
}

uint32_t exe_orr(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  return operand1 | operand2;
}

uint32_t exe_mov(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  return operand2;
}

// Generates an error message if OpCode is incorrect
uint32_t exe_error(uint32_t operand1, uint32_t operand2, uint8_t *carry) {
  fprintf(stderr, "Error : unknown OpCode");
  exit(EXIT_FAILURE);
}

void execute_multiply(Emulator *emulator) {
  Instruction instruction = emulator->decoded;
  uint64_t result = emulator->registers[instruction.rm] * emulator->registers[instruction.rs];

  if (instruction.a) {
    // if instruction is mla, add contents of rn
    result += emulator->registers[instruction.rn];
  }

  emulator->registers[instruction.rd] = result;
  set_cpsr(emulator, result, 0);
}

void execute_single_data_transfer(Emulator *emulator) {
  uint32_t offset = get_offset_singledt(emulator);
  Instruction instruction = emulator->decoded;
  uint32_t address = emulator->registers[instruction.rn];

  if (instruction.p) {
    // Pre-indexing so apply offset first
    address += (instruction.u ? offset : -offset);
  }

  bool gpio_flag = false;

  if (address >= 65536) {
    switch (address) {
      case (0x20200008):
        printf("%s", "One GPIO pin from 20 to 29 has been accessed\n");
        gpio_flag = true;
        break;
      case (0x20200004):
        printf("%s", "One GPIO pin from 10 to 19 has been accessed\n");
        gpio_flag = true;
        break;
      case (0x20200000):
        printf("%s", "One GPIO pin from 0 to 9 has been accessed\n");
        gpio_flag = true;
        break;
      case (0x20200028):
        printf("%s", "PIN OFF\n");
        return;
      case 0x2020001C:
        printf("%s", "PIN ON\n");
        return;
      default:
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return;
    }
  }

  if (instruction.l) {
    // Loading into destination register
    if (gpio_flag) {
      emulator->registers[instruction.rd] = address;
    } else {
      emulator->registers[instruction.rd] = get_memory(emulator, address);
    }
  } else {
    if (!gpio_flag) {
      // Storing into destination register
      set_memory(emulator, address,
                 emulator->registers[instruction.rd]);
    }
  }

  if (!(instruction.p)) {
    // Post-indexing so apply offset last
    emulator->registers[instruction.rn] += (instruction.u ? offset : -offset);
  }
}

uint32_t get_offset_singledt(Emulator *emulator) {
  Instruction *instruction = &emulator->decoded;

  if (!instruction->i) {
    // Offset is an immediate value
    return instruction->offset;
  } else {
    // Offset is a register
    uint32_t rm = emulator->registers[instruction->rm];
    uint32_t shift;

    if (filter_bits(instruction->offset, 1, 4)) {
      // Shift specified by a register
      // The bottom byte of Rs specifies the amount to be shifted
      shift = filter_bits(emulator->registers[instruction->rs], 8, 0);
    } else {
      // Shift by a constant amount
      shift = filter_bits(instruction->offset, 5, 7);
    }

    // Returns the shifted value of rm
    // Executes appropriate shift operation according to shift type
    if (shift) {
      return exe_shift_ptr[filter_bits(instruction->offset, 2, 5)](rm, shift);
    }

    // Returns the value of rm
    return rm;
  }
}

void execute_branch(Emulator *emulator) {
  int32_t offset = emulator->decoded.offset;
  // Check if signbit is set
  if (filter_bits(offset, 1, 23)) {
    // Subtracted twice to consider for initially added bit
    offset -= (1 << 24);
  }
  // Offset is shifted twice.
  offset = offset << 2;

  // Offset is added to PC
  emulator->registers[PC] += offset;
  emulator->execute_flag = false;
  emulator->decode_flag = false;
}

void set_cpsr(Emulator *emulator, uint32_t result, uint8_t carry) {
  emulator->registers[CPSR] = 0;
  if (carry) {
    // Setting C
    emulator->registers[CPSR] = set_bits(emulator->registers[CPSR], 1, 29);
  }

  if (result == 0) {
    // Setting Z
    emulator->registers[CPSR] = set_bits(emulator->registers[CPSR], 1, 30);
  }

  if (filter_bits(result, 1, 31) == 1) {
    // Setting N
    emulator->registers[CPSR] = set_bits(emulator->registers[CPSR], 1, 31);
  }
}

bool check_cond(Emulator *emulator) {
  uint32_t cpsr_register = emulator->registers[CPSR];
  uint32_t N = filter_bits(cpsr_register, 1, 31);
  uint32_t Z = filter_bits(cpsr_register, 1, 30);
  // bit C is never checked
  // uint32_t C = filter_bits(cpsr_register, 1, 29);
  uint32_t V = filter_bits(cpsr_register, 1, 28);

  switch (emulator->decoded.cond) {
    case (EQ):
      return (Z != 0);
    case (NE) :
      return (Z == 0);
    case (GE) :
      return (N == V);
    case (LT) :
      return (N != V);
    case (GT) :
      return (Z == 0 && N == V);
    case (LE) :
      return (Z != 0 || (N != V));
      // AL
    default:
      return true;
  }
}