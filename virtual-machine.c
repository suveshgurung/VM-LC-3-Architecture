#include "virtual-machine.h"
#include <stdio.h>

uint16_t memory[MAX_MEMORY_SIZE];
uint16_t registers[R_COUNT] = {0};

void bin(unsigned n) {
  /* step 1 */
  if (n > 1)
    bin(n / 2);

  /* step 2 */
  printf("%d", n % 2);
}

int main(int argc, char *argv[]) {
  // uint16_t instruction = 0b0101110010111100;
  // uint16_t instruction = 0b0010010011111001;
  // uint16_t instruction = 0b1001010010111111;
  // uint16_t instruction = 0b0110001010111101;
  uint16_t instruction = 0b0000101111110101;
  registers[R_PC] = 0xffff;
  registers[R_COND] = COND_ZERO;
  registers[R_2] = 0xffff;
  registers[R_6] = -101;
  struct decoded_instruction d_instruction = decode_instruction(instruction);

  if (d_instruction.opcode == OP_ADD) {
    operate_add(d_instruction);
    print_add_result(d_instruction);
  } 
  else if (d_instruction.opcode == OP_AND) {
    operate_and(d_instruction);
    print_and_result(d_instruction);
  }
  else if (d_instruction.opcode == OP_NOT) {
    operate_not(d_instruction);
    print_not_result(d_instruction);
  }
  else if (d_instruction.opcode == OP_LEA) {
    operate_lea(d_instruction);
  }
  else if (d_instruction.opcode == OP_LD) {
    operate_ld(d_instruction);
  }
  else if (d_instruction.opcode == OP_LDI) {
    operate_ldi(d_instruction);
  }
  else if (d_instruction.opcode == OP_LDR) {
    operate_ldr(d_instruction);
  }
  else if (d_instruction.opcode == OP_ST) {
    operate_st(d_instruction);
  }
  else if (d_instruction.opcode == OP_STI) {
    operate_sti(d_instruction);
  }
  else if (d_instruction.opcode == OP_STR) {
    operate_str(d_instruction);
  }
  else if (d_instruction.opcode == OP_BR) {
    operate_br(d_instruction);
  }

  return 0;
}

struct decoded_instruction decode_instruction(uint16_t instruction) {
  struct decoded_instruction d_instruction;

  /* common for all instructions. */
  d_instruction.opcode = instruction >> 12;

  switch (d_instruction.opcode) {
    case OP_ADD:
    case OP_AND:
      d_instruction.add_and_instruction.dest = (uint8_t)((instruction >> 9) & 0x0007);
      d_instruction.add_and_instruction.src1 = (uint8_t)((instruction >> 6) & 0x0007);
      if (is_immediate_addressing_mode(instruction)) {
        d_instruction.add_and_instruction.mode = (uint8_t)MOD_IMM;
        if (is_positive_immediate_value(instruction)) {
          /* sign extended 16-bit positive 2's complement integer. */
          d_instruction.add_and_instruction.src2 = instruction & 0x001f;
        } 
        else {
          /* sign extended 16-bit negative 2's complement integer. */
          d_instruction.add_and_instruction.src2 = (instruction & 0x001f) | 0xffe0;
        }
      }
      else {
        d_instruction.add_and_instruction.mode = MOD_REG;
        d_instruction.add_and_instruction.src2 = instruction & 0x0007;
      }
      break;
    case OP_NOT:
      d_instruction.not_instruction.dest = (uint8_t)((instruction >> 9) & 0x0007);
      d_instruction.not_instruction.src = (uint8_t)((instruction >> 6) & 0x0007);
      break;
    case OP_LEA:
    case OP_LD:
    case OP_LDI:
      d_instruction.ld_ldi_instruction.dest = (uint8_t)((instruction >> 9) & 0x0007);
      if (is_positive_offset_value(instruction, 9, 0)) {
        d_instruction.ld_ldi_instruction.offset = instruction & 0x01ff;
      }
      else {
        d_instruction.ld_ldi_instruction.offset = (instruction & 0x01ff) | 0xfe00;
      }
      break;
    case OP_LDR:
      d_instruction.ldr_instruction.dest = (uint8_t)((instruction >> 9) & 0x0007);
      d_instruction.ldr_instruction.base = (uint8_t)((instruction >> 6) & 0x0007);
      if (is_positive_offset_value(instruction, 6, 0)) {
        d_instruction.ldr_instruction.offset = (instruction & 0x003f);
      }
      else {
        d_instruction.ldr_instruction.offset = (instruction & 0x003f) | 0xffc0;
      }
      break;
    case OP_ST:
    case OP_STI:
      d_instruction.st_sti_instruction.src = (uint8_t)((instruction >> 9) | 0x0007);
      if (is_positive_offset_value(instruction, 9, 0)) {
        d_instruction.st_sti_instruction.offset = instruction & 0x01ff;
      }
      else {
        d_instruction.st_sti_instruction.offset = (instruction & 0x01ff) | 0xfe00;
      }
      break;
    case OP_STR:
      d_instruction.str_instruction.src = (uint8_t)((instruction >> 9) & 0x0007);
      d_instruction.str_instruction.base = (uint8_t)((instruction >> 6) & 0x0007);
      if (is_positive_offset_value(instruction, 6, 0)) {
        d_instruction.str_instruction.offset = (instruction & 0x003f);
      }
      else {
        d_instruction.str_instruction.offset = (instruction & 0x003f) | 0xffc0;
      }
      break;
    case OP_BR:
      d_instruction.br_instruction.condition = (uint8_t)((instruction >> 9) & 0x0007);
      if (is_positive_offset_value(instruction, 9, 0)) {
        d_instruction.br_instruction.offset = instruction & 0x01ff;
      } 
      else {
        d_instruction.br_instruction.offset = (instruction & 0x01ff) | 0xfe00;
      }
      break;
    default:
      break;
  }

  return d_instruction;
}

enum npz check_npz(uint16_t value) {
  if (value == 0) {
    return COND_ZERO;
  }
  else if (is_negative_number(value)) {
    return COND_NEG;
  }
  else {
    return COND_POS;
  }
}

bool is_immediate_addressing_mode(uint16_t instruction) {
  /* bit 5 represents the mode */
  if ((instruction >> 5) & 0x0001) {
    return true;
  }
  return false;
}

bool is_positive_immediate_value(uint16_t instruction) {
  /* check if the MSB of the 5 bit immediate value is 0 */
  if (!((instruction >> 4) & 0x0001)) {
    return true;
  }
  return false;
}

bool is_positive_offset_value(uint16_t instruction, int offset_bits, int offset_start) {
  int sign_bit_position = offset_bits + offset_start - 1;
  int sign_bit_mask = 1 << sign_bit_position;

  if (!(instruction & sign_bit_mask)) {
    return true;
  }
  else {
    return false;
  }
}

bool is_negative_number(uint16_t number) {
  if ((number >> 15)) {
    return true;
  }
  return false;
}

/* temporarily needed functions */
uint16_t conv_negative_to_positive_int(uint16_t negative_number) {
  uint16_t ones_complement_number = ~negative_number;
  uint16_t positive_number =
    ones_complement_number + 0x0001; /* calculate the 2's complement */

  return positive_number;
}
/* */

void operate_add(struct decoded_instruction d_instruction) {
  uint16_t first_operand = registers[d_instruction.add_and_instruction.src1];
  uint16_t second_operand = 0;
  if (d_instruction.add_and_instruction.mode == MOD_IMM) {
    second_operand = d_instruction.add_and_instruction.src2;
  }
  else if (d_instruction.add_and_instruction.mode == MOD_REG) {
    second_operand = registers[d_instruction.add_and_instruction.src2];
  }

  registers[d_instruction.add_and_instruction.dest] = first_operand + second_operand;
  registers[R_COND] = check_npz(registers[d_instruction.add_and_instruction.dest]);
}

void print_add_result(struct decoded_instruction d_instruction) {
  if (is_negative_number(registers[d_instruction.add_and_instruction.dest])) {
    printf("Result of add: -%d\n",
           conv_negative_to_positive_int(
           registers[d_instruction.add_and_instruction.dest]));
  } else {
    printf("Result of add: %d\n",
           registers[d_instruction.add_and_instruction.dest]);
  }
}

void operate_and(struct decoded_instruction d_instruction) {
  uint16_t first_operand = registers[d_instruction.add_and_instruction.src1];
  uint16_t second_operand = 0;
  if (d_instruction.add_and_instruction.mode == MOD_IMM) {
    second_operand = d_instruction.add_and_instruction.src2;
  }
  else if (d_instruction.add_and_instruction.mode == MOD_REG) {
    second_operand = registers[d_instruction.add_and_instruction.src2];
  }

  registers[d_instruction.add_and_instruction.dest] = first_operand & second_operand;
  registers[R_COND] = check_npz(registers[d_instruction.add_and_instruction.dest]);
}

void print_and_result(struct decoded_instruction d_instruction) {
  if (is_negative_number(registers[d_instruction.add_and_instruction.dest])) {
    printf("Result of and: -%d\n",
           conv_negative_to_positive_int(
           registers[d_instruction.add_and_instruction.dest]));
  } 
  else {
    printf("Result of and: %d\n",
           registers[d_instruction.add_and_instruction.dest]);
  }
}

void operate_not(struct decoded_instruction d_instruction) {
  registers[d_instruction.not_instruction.dest] = ~registers[d_instruction.not_instruction.src];
  registers[R_COND] = check_npz(registers[d_instruction.not_instruction.dest]);
}

void print_not_result(struct decoded_instruction d_instruction) {
  if (is_negative_number(registers[d_instruction.not_instruction.dest])) {
    printf("Result of not: -%d\n",
           conv_negative_to_positive_int(
           registers[d_instruction.not_instruction.dest]));
  }
  else {
    printf("Result of not: %d\n",
           registers[d_instruction.not_instruction.dest]);
  }
}

void operate_lea(struct decoded_instruction d_instruction) {
  registers[d_instruction.lea_instruction.dest] = registers[R_PC] + d_instruction.lea_instruction.offset;
}

void operate_ld(struct decoded_instruction d_instruction) {
  registers[d_instruction.ld_ldi_instruction.dest] = memory[registers[R_PC] + d_instruction.ld_ldi_instruction.offset];
  registers[R_COND] = check_npz(registers[d_instruction.ld_ldi_instruction.dest]);
}

void operate_ldi(struct decoded_instruction d_instruction) {
  uint16_t mem_address = memory[registers[R_PC] + d_instruction.ld_ldi_instruction.offset];
  registers[d_instruction.ld_ldi_instruction.dest] = memory[mem_address];
  registers[R_COND] = check_npz(registers[d_instruction.ld_ldi_instruction.dest]);
}

void operate_ldr(struct decoded_instruction d_instruction) {
  uint16_t mem_address = registers[d_instruction.ldr_instruction.base] + d_instruction.ldr_instruction.offset;
  registers[d_instruction.ldr_instruction.dest] = memory[mem_address];
  registers[R_COND] = check_npz(registers[d_instruction.ldr_instruction.dest]);
}

void operate_st(struct decoded_instruction d_instruction) {
  memory[registers[R_PC] + d_instruction.st_sti_instruction.offset] = registers[d_instruction.st_sti_instruction.src];
}

void operate_sti(struct decoded_instruction d_instruction) {
  uint16_t mem_address = memory[registers[R_PC] + d_instruction.st_sti_instruction.offset];
  memory[mem_address] = registers[d_instruction.st_sti_instruction.src];
}

void operate_str(struct decoded_instruction d_instruction) {
  uint16_t mem_address = registers[d_instruction.str_instruction.base] + d_instruction.str_instruction.offset;
  memory[mem_address] = registers[d_instruction.str_instruction.src];
}

// TODO: write this function later on.
bool check_br_condition(uint8_t condition) {
  /* |2|1|0| */
  /* |n|z|p| */
  for (int shift = 0; shift <=2; shift++) {
    if ((condition >> shift) & 0x01) {
      switch (shift) {
        case 0:
          if (registers[R_COND] == COND_POS) {
            return true;
          }
          break;
        case 1:
          if (registers[R_COND] == COND_ZERO) {
            return true;
          }
          break;
        case 2:
          if (registers[R_COND] == COND_NEG) {
            return true;
          }
          break;
        default:
          break;
      }
    }
  }

  return false;
}

void operate_br(struct decoded_instruction d_instruction) {
  if (check_br_condition(d_instruction.br_instruction.condition)) {
    registers[R_PC] = registers[R_PC] + d_instruction.br_instruction.offset;
  }
}
