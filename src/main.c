#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

int main(int argc, char* argv[])
{
  FILE* code;
  FILE* svg;

  // Input error handling
  if (argc == 3)
  {
    code = fopen(argv[1], "r");
    svg = fopen(argv[2], "w");
  }
  else
  {
    fprintf(stderr, "Usage: %s <instruction_file> <svg_output>\n", argv[0]);
    throw_error("");
  }

  char* a = malloc(sizeof(char) * 10);
  int i = 0;

  // reading and parsing instructions until file end
  while (fscanf(code, "%s", a) != EOF)
  {
    int num = (int)strtol(a, NULL, 16);
    program[i++] = instruction_parse(num);
  }
  free(a);

  // Initializing MAX_PC
  MAX_PC = (4 * (i - 1)) + BASE_PC_ADDR;

  // // Printing all instructions
  // for (i = BASE_PC_ADDR; i <= MAX_PC; i += 4)
  // {
  //   print_instruction(&program[(i - BASE_PC_ADDR) / 4]);
  // }

  // Initializing memory and pipeline buffer
  for (i = 0; i <= (NUM_THREADS - 1); i++)
  {
    pipeline[i].instr.Itype = NO_OP;
    pipeline[i].instr.Ctype = NO_OPERATION;
  }

  for (i = 0; i < MEMORY_SIZE; i++)
  {
    // Memory_Block[i] = 1;  // Just for checking
    Memory_Block[i] = 0;
  }

  // Initialize register file
  for (i = 0; i < 32; i++)
  {
    // register_file[i] = i + 1;  // Just for checking
    register_file[i] = 0;
  }
  control_signal.stall=0;
  control_signal.branched=0;

  // Initializing mutex locks
  pthread_mutex_init(&CLOCK_LOCK, NULL);
  pthread_mutex_init(&READ_LOCK, NULL);
  pthread_mutex_init(&WRITE_LOCK, NULL);

  // Creating threads
  pthread_create(&threads[0], NULL, instruction_fetch, (void*)NULL);
  pthread_create(&threads[1], NULL, register_read, (void*)NULL);
  pthread_create(&threads[2], NULL, alu_op, (void*)NULL);
  pthread_create(&threads[3], NULL, memory_op, (void*)NULL);
  pthread_create(&threads[4], NULL, register_write, (void*)NULL);
  // pthread_create(&threads[5], NULL, print_svg, (void*)NULL);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  pthread_join(threads[2], NULL);
  pthread_join(threads[3], NULL);
  pthread_join(threads[4], NULL);
  // pthread_join(threads[5], NULL);

  fclose(code);
  fclose(svg);
  return 0;
}