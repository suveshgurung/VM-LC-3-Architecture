#include "virtual-machine.h"
#include <stdio.h>


uint16_t memory[MAX_MEMORY_SIZE];
uint16_t registers[R_COUNT];


int main(int argc, char *argv[]) {
  uint16_t instruction = 0b0101010011100000;
  struct decoded_instruction d_instruction = decode_instruction(instruction);
  if (d_instruction.opcode == OP_AND) {
    printf("The opcode is a \"AND\" opcode!\n");
    if (is_immediate_addressing_mode(instruction)) {
      printf("The addressing mode is \"immediate\"\n");
    }
  }
  return 0;
}

struct decoded_instruction decode_instruction(uint16_t instruction) {
  struct decoded_instruction d_instruction;

  d_instruction.opcode = instruction >> 12;
  d_instruction.destination_register = (instruction >> 9) & 0x0007;
  d_instruction.first_source_register = (instruction >> 6) & 0x0007;
  if (is_immediate_addressing_mode(instruction)) {
    if (is_positive_immediate_value(instruction)) {
      // positive 2's complement integer.
      d_instruction.immediate_value = instruction & 0x001f;
    } else {
      // negative 2's complemet integer.
      d_instruction.immediate_value = (instruction & 0x001f) | 0xffe0;
    }
  } else {
    d_instruction.second_source_register = instruction & 0x0007;
  }

  return d_instruction;
}

bool is_immediate_addressing_mode(uint16_t instruction) {
  if ((instruction >> 5) & 0x0001) {
    return true;
  }
  return false;
}

bool is_positive_immediate_value(uint16_t instruction) {
  if (!((instruction >> 4) & 0x0001)) {
    return true;
  }
  return false;
}

void add(uint16_t instruction) {

}
