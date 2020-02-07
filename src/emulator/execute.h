#ifndef ARM11_35_EXECUTE_H
#define ARM11_35_EXECUTE_H

#include "emulator.h"

void execute(Emulator *emulator);

void execute_data_processing(Emulator *emulator);

void execute_multiply(Emulator *emulator);

void execute_single_data_transfer(Emulator *emulator);

void execute_branch(Emulator *emulator);

void set_cpsr(Emulator *emulator, uint32_t result, uint8_t carry);

bool check_cond(Emulator *emulator);

uint32_t logical_left(uint32_t x, uint32_t n);

uint32_t logical_right(uint32_t x, uint32_t n);

uint32_t arithmetic_right(uint32_t x, uint32_t n);

uint32_t rotate_right(uint32_t x, uint32_t n);

uint32_t get_operand_datap(Emulator *emulator, uint8_t *carry);

uint32_t get_offset_singledt(Emulator *emulator);

uint32_t exe_and(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_eor(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_sub(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_rsb(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_add(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_orr(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_mov(uint32_t operand1, uint32_t operand2, uint8_t *carry);

uint32_t exe_error(uint32_t operand1, uint32_t operand2, uint8_t *carry);

#endif //ARM11_35_EXECUTE_H
