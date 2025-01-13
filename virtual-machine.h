#include <stdint.h>
#include <stdbool.h>

#define MAX_MEMORY_SIZE (1 << 16)
extern uint16_t memory[MAX_MEMORY_SIZE];     /* 65536 memory locations */

/* registers */
enum {
  R_0 = 0,
  R_1,
  R_2,
  R_3,
  R_4,
  R_5,
  R_6,
  R_7,
  R_PC,
  R_COND,     /* can contain the values COND_POS, COND_ZERO, COND_NEG */
  R_COUNT     /* total number of registers = 10 */
};
extern uint16_t registers[R_COUNT];

/* opcodes */
enum {
  OP_BR = 0,  /* BR = 0 0 0 0 */
  OP_ADD,     /* ADD = 0 0 0 1 */
  OP_LD,      /* LD = 0 0 1 0 */
  OP_ST,      /* ST = 0 0 1 1 */
  OP_JSR,     /* JSR = 0 1 0 0 */
  OP_AND,     /* AND = 0 1 0 1 */
  OP_LDR,     /* LDR = 0 1 1 0 */
  OP_STR,     /* STR = 0 1 1 1 */
  OP_RTI,     /* RTI = 1 0 0 0 */
  OP_NOT,     /* NOT = 1 0 0 1 */
  OP_LDI,     /* LDI = 1 0 1 0 */
  OP_STI,     /* STI = 1 0 1 1 */
  OP_JMP,     /* RET = 1 1 0 0 */
  OP_RES,     /* RES = 1 1 0 1 (unused) */
  OP_LEA,     /* LEA = 1 1 1 0 */
  OP_TRAP     /* TRAP = 1 1 1 1 */
};

/* condition codes */
enum {
  COND_POS = 1 << 0,      /* P */
  COND_ZERO = 1 << 1,     /* Z */
  COND_NEG = 1 << 2       /* N */
};



/* structs */



/* function prototypes */
uint16_t decode_instruction(uint16_t instruction);
bool is_immediate_addressing_mode(uint16_t instruction);
