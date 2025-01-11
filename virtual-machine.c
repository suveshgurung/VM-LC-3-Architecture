#include <stdint.h>

#define MAX_MEMORY_SIZE (1 << 16)
uint16_t memory[MAX_MEMORY_SIZE];     /* 65536 memory locations */

/* registers */
enum {
  R_0,
  R_1,
  R_2,
  R_3,
  R_4,
  R_5,
  R_6,
  R_7,
  R_PC,
  R_COND,
  R_COUNT     /* total number of registers = 10 */
};
uint16_t registers[R_COUNT];

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
  OP_RES,     /* RES = 1 1 0 1 */
  OP_LEA,     /* LEA = 1 1 1 0 */
  OP_TRAP     /* TRAP = 1 1 1 1 */
};
