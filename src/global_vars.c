#include "global_vars.h"

buffer pipeline[NUM_THREADS - 1];

instruction program[INSTRUCTION_MEM];

int register_file[32];
int PC = 0;
int MAX_PC = 0;
int STEPS = 0;
int CLOCK = 0;  // emulates the clock which all of the instructions follow
double FREQUENCY = 0;
unsigned int DELAY = 1000;  // sleep delay for each thread in micro seconds
int NUM_THREADS_READ =
    0;  // number of threads that have completed the copy stage
        // from buffer
int NUM_THREADS_WRITE =
    0;  // number of threads that have completed processing for
        // that particular cycle
pthread_t threads[NUM_THREADS];

int Memory_Block[MEMORY_SIZE];

pthread_mutex_t CLOCK_LOCK;
pthread_mutex_t READ_LOCK;
pthread_mutex_t WRITE_LOCK;

signal control_signal;
