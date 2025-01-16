#include "virtual-machine.h"
#include <stdio.h>


uint16_t memory[MAX_MEMORY_SIZE];
uint16_t registers[R_COUNT] = { 0 };

void bin(unsigned n)
{
    /* step 1 */
    if (n > 1)
        bin(n / 2);

    /* step 2 */
    printf("%d", n % 2);
}

int main(int argc, char *argv[]) {
  uint16_t instruction = 0b0001110010000110;
  registers[R_2] = 6;
  registers[R_6] = -12;
  bin(registers[R_2]);
  printf("\n");
  bin(registers[R_6]);
  printf("\n");
  struct decoded_instruction d_instruction = decode_instruction(instruction);
  if (d_instruction.opcode == OP_ADD) {
    // printf("The opcode is a \"AND\" opcode!\n");
    // if (d_instruction.instruction_mode == I_IMM) {
    //   printf("The addressing mode is \"immediate\"\n");
    // }
    add(d_instruction);
    printf("Result of add: %d\n", registers[R_6]);
  }
  return 0;
}

struct decoded_instruction decode_instruction(uint16_t instruction) {
  struct decoded_instruction d_instruction;

  d_instruction.opcode = instruction >> 12;
  d_instruction.destination_register = (instruction >> 9) & 0x0007;
  d_instruction.first_source_register = (instruction >> 6) & 0x0007;
  if (is_immediate_addressing_mode(instruction)) {
    d_instruction.instruction_mode = MOD_IMM;
    if (is_positive_immediate_value(instruction)) {
      /* positive 2's complement integer. */
      d_instruction.immediate_value = instruction & 0x001f;
    } else {
      /* negative 2's complemet integer. */
      d_instruction.immediate_value = (instruction & 0x001f) | 0xffe0;
    }
  } else {
    d_instruction.instruction_mode = MOD_REG;
    d_instruction.second_source_register = instruction & 0x0007;
  }

  return d_instruction;
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

bool is_negative_number(uint16_t number) {
  if (!(number >> 15)) {
    return true;
  }
  return false;
}

uint16_t conv_negative_to_positive_int(uint16_t negative_number) {
  uint16_t ones_complement_number = ~negative_number;
}

void add(struct decoded_instruction d_instruction) {
  if (d_instruction.instruction_mode == MOD_IMM) {
    // registers[d_instruction.destination_register] = registers[d_instruction.first_source_register] - conv_neg_to_pos_int(d_instruction.immediate_value);
    registers[d_instruction.destination_register] = registers[d_instruction.first_source_register] + d_instruction.immediate_value;
  } else if (d_instruction.instruction_mode == MOD_REG) {
    uint16_t first_operand = registers[d_instruction.first_source_register];
    uint16_t second_operand = registers[d_instruction.second_source_register];
    if (is_negative_number(first_operand)) {
      first_operand = conv_negative_to_positive_int(first_operand);
    }
    if (is_negative_number(second_operand)) {
      second_operand = conv_negative_to_positive_int(second_operand);
    }
    registers[d_instruction.destination_register] = first_operand + ;
  }
}
