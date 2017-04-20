#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <pthread.h>

/**
* Instruction memory
*/
#define INSTRUCTION_MEM 0x00010000
#define NUM_THREADS 5
#define MEMORY_SIZE 1671168  // equals 64MB of memory
#define BASE_ADDR 0x10010000
#define END_ADDR 0x14010000
#define BASE_PC_ADDR 0x00400000
#define END_PC_ADDR 0x00410000

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
  JUMP,
  JUMP_LINK,
  JUMP_REGISTER,
  JUMP_LINK_REGISTER,
  MFLO,
  NO_OP
} instruction_type;

typedef enum { DP, DT, BRANCH, NO_OPERATION } class_type;

typedef struct
{
  int rs, rt, rd, shf_amt, opcode, target_address, function, immediate;
  instruction_type Itype;
  class_type Ctype;
  int index;
} instruction;

typedef struct buffer
{
  instruction instr;
  int rs_val, rt_val, rd_val, alu_result, HI, LO, pc;
} buffer;

typedef struct
{
  int stall;
  int branched;
} signal;

typedef struct
{
  int MemWr, MemRd, PCsrc, RegW, FWD_ALU, FWD_DM, TO_ALU, TO_DM, M2R, FLUSH,
      STALL_C;
} control;

extern control CONTROL_SIGN;
extern buffer pipeline[NUM_THREADS - 1];
extern buffer temp_pipeline[NUM_THREADS - 1];
extern instruction program[INSTRUCTION_MEM];
extern instruction CURR_INSTR[NUM_THREADS];
extern signal control_signal;

// register_file[32]= LO && register_file[33] = HIGH
extern int register_file[34];
extern int STOP_THREAD;
extern int INSTRUCTION_COUNT;
extern int DATA_MEM_ACCESS;
extern int INSTRUCTION_MEM_ACCESS;
extern int STALL_COUNT;
extern int BRANCH_CYCLE_WASTE;
extern int PC;
extern int MAX_PC;
extern int STEPS;
extern int CLOCK;  // emulates the clock which all of the instructions follow
extern double FREQUENCY;
extern unsigned int DELAY;  // sleep delay for each thread in micro seconds
extern int Memory_Block[MEMORY_SIZE];
extern int BreakPoint[INSTRUCTION_MEM];
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

extern float SVG_HEIGHT;
extern float SVG_WIDTH;
extern float SVG_SATURATE;

// Display signals
extern int ACTIVE_STAGE[5];    // [0..4]-> fetch,decode,alu,mem,write
extern int FORWARDING[3];      // [0..2] -> alu-alu, mem-alu, mem-mem
extern int FORWARDING_ALU[2];  // Forwarding to rt or rs
#endif
