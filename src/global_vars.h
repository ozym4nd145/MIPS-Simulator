#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <pthread.h>

/**
* Instruction memory
*/
#define INSTRUCTION_MEM 10000
#define NUM_THREADS 5
#define MEMORY_SIZE 1000000

typedef enum {
  ADD,
  ADDI,
  SUB,
  AND,
  MULTIPLY,
  MULTIPLY_ADD,
  NOR,
  OR,
  ORI,
  LOGIC_SHIFT_LEFT,
  LOGIC_SHIFT_LEFT_VARIABLE,
  BRANCH_GREATER_OR_EQUAL,
  BRANCH_GREATER,
  BRANCH_LESS_OR_EQUAL,
  BRANCH_LESS,
  BRANCH_EQUAL,
  LDR_BYTE,
  LDR_WORD,
  STR_BYTE,
  STR_WORD,
  SLTU,
  SLTI,
  LDR_UPPER_IMMEDIATE,
  NO_OP
} instruction_type;

typedef enum { DP, DT, BRANCH, NO_OPERATION } class_type;

typedef struct
{
  int rs, rt, rd, shf_amt, opcode, target_address, function, immediate;
  instruction_type Itype;
  class_type Ctype;
} instruction;

typedef struct buffer
{
  instruction instr;
  int rs_val, rt_val, rd_val, alu_result, HI, LO, pc;
} buffer;

typedef struct
{
  int stall;
} signal;

extern buffer pipeline[NUM_THREADS - 1];
extern buffer temp_pipeline[NUM_THREADS - 2];
extern instruction program[INSTRUCTION_MEM];
extern signal control_signal;

extern int register_file[32];
extern int PC;
extern int MAX_PC;
extern int STEPS;
extern int CLOCK;  // emulates the clock which all of the instructions follow
extern double FREQUENCY;
extern unsigned int DELAY;  // sleep delay for each thread in micro seconds
extern int Memory_Block[MEMORY_SIZE];
extern int
    NUM_THREADS_READ;  // number of threads that have completed the copy stage
                       // from buffer
extern int
    NUM_THREADS_WRITE;  // number of threads that have completed processing for
                        // that particular cycle

extern pthread_t threads[NUM_THREADS];
extern pthread_mutex_t CLOCK_LOCK;
extern pthread_mutex_t READ_LOCK;
extern pthread_mutex_t WRITE_LOCK;

#endif