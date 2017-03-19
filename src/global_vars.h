#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <pthread.h>

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
  int rs_val,rt_val,rd_val,alu_result,HI,LO;
} buffer;

typedef struct
{
  int stall;

} signal;

extern buffer pipeline[NUM_PIPELINES];
extern instruction program[INSTRUCTION_MEM];
extern int register_file[32];
extern int PC;
extern int MAX_PC;  // in bytes == 4*number of instructions
extern int STEPS;   // counts the numbers of steps taken
extern pthread_t threads[NUM_PIPELINES];  // Threads corresponding to different
                                          // pipeline threads
extern signal control_signal;

#endif