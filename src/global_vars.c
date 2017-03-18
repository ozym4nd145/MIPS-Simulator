#include "global_vars.h"

buffer pipeline[NUM_PIPELINES];

instruction program[INSTRUCTION_MEM];

int register_file[32];
int PC;
int MAX_PC;