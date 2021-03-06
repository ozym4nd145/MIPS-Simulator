#include "global_vars.h"

buffer pipeline[NUM_THREADS - 1];       // Pipelines in between different stages
buffer temp_pipeline[NUM_THREADS - 1];  // Temporary pipeline for reading stage
instruction program[INSTRUCTION_MEM];   // Array containing decoded instructions
signal control_signal;                  // Control signals

instruction CURR_INSTR[NUM_THREADS];  // Lastest instruction read by
                                      // each stage

int register_file[34];  // Register file
int PC = BASE_PC_ADDR;  // Global value of pc
int MAX_PC = 0;         // Max value of pc that can be attained
int STEPS = 0;          // Number of clock cycles passed
int CLOCK = 0;        // emulates the clock which all of the instructions follow
int STOP_THREAD = 0;  // Signal to highlight end of threads
int INSTRUCTION_COUNT = 0;  // Counts total instructions executed
double FREQUENCY = 0;
int DATA_MEM_ACCESS = 0;
int STALL_COUNT = 0;
int BRANCH_CYCLE_WASTE = 0;
int INSTRUCTION_MEM_ACCESS = 0;
unsigned int DELAY = 1;          // sleep delay for each thread in micro seconds
int LATENCY = 0;                 // Latency of memory access in cycles
int NUM_THREADS_READ = 0;        // threads that have completed the copy stage
                                 // from buffer
int NUM_THREADS_WRITE = 0;       // threads that have completed processing for
                                 // that particular cycle
pthread_t threads[NUM_THREADS];  // Totals running threads
int BreakPoint[INSTRUCTION_MEM];

control CONTROL_SIGN;

int Memory_Block[MEMORY_SIZE];  // Memory Block
int stall_BreakPoint = 0;

pthread_mutex_t CLOCK_LOCK;  // Lock for syncing clock signal
pthread_mutex_t READ_LOCK;   // Lock for syncing read update of all threads
pthread_mutex_t WRITE_LOCK;  // Lock for syncing write update of all threads

float SVG_HEIGHT = 210;
float SVG_WIDTH = 297;
float SVG_SATURATE = 0.0;

// Display signals
int ACTIVE_STAGE[5];  // [0..4]-> fetch,decode,alu,mem,write
int CLOCK_ZERO_READ[5] = {1, 1, 1, 1,
                          1};  // [0..4]-> decode,alu,mem,write,display
int SVG_WRITTEN = 1;

FILE* trace_file;