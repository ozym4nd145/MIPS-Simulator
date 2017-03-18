#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

/**
* Instruction memory
*/
#define INSTRUCTION_MEM 10000
#define NUM_PIPELINES 5

typedef enum {
  ADD,
  SUB,
  AND,
  MULTIPLY,
  MULTIPLY_ADD,
  NOR,
  OR,
  LOGIC_SHIFT_LEFT,
  LOGIC_SHIFT_LEFT_VARIABLE,
  SUBTRACT,
  BRANCH_GREATER_OR_EQUAL,
  BRANCH_GREATER,
  BRANCH_LESS_OR_EQUAL,
  BRANCH_LESS,
  BRANCH_EQUAL,
  LDR_BYTE,
  LDR_WORD,
  STR_BYTE,
  STR_WORD
} instruction_type;

typedef enum { DP, DT, BRANCH } class_type;

typedef struct
{
  int rs, rt, rd, shf_amt, opcode, target_address, function, immediate;
  instruction_type Itype;
  class_type Ctype;
} instruction;

typedef struct buffer
{
  instruction instr;
  int rs_val,rt_val,rd_val,alu_result;
} buffer;

extern buffer pipeline[NUM_PIPELINES];
extern instruction program[INSTRUCTION_MEM];
extern int register_file[32];
extern int PC;
extern int MAX_PC;  // in bytes == 4*number of instructions
#endif