#include "virtual-machine.h"
#include <stdio.h>


uint16_t memory[MAX_MEMORY_SIZE];
uint16_t registers[R_COUNT];


int main(int argc, char *argv[]) {
  uint16_t instruction = 0b0101010011100000;
  uint16_t opcode = decode_instruction(instruction);
  if (opcode == OP_AND) {
    printf("The opcode is a \"AND\" opcode!\n");
    if (is_immediate_addressing_mode(instruction)) {
      printf("The addressing mode is \"immediate\"\n");
    }
  }
  return 0;
}

uint16_t decode_instruction(uint16_t instruction) {
  uint16_t opcode = instruction >> 12;

  return opcode;
}

bool is_immediate_addressing_mode(uint16_t instruction) {
  if ((instruction >> 5) & 0x0001) {
    return true;
  }
  return false;
}
