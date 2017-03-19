#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* instruction_fetch(void* data)
{
  printf("Inside Instruction fetch\n");
  char input[100];

  while (1)
  {
    scanf("%s", input);
    if (strcmp(input, "step") == 0)
    {
      STEPS++;

      // lock CLOCK for updating
      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 1;
      pthread_mutex_unlock(&CLOCK_LOCK);

      int stall = control_signal.stall;

      // loop until reading stage has completed
      int loop = 1;
      while (loop)
      {
        usleep(DELAY);
        pthread_mutex_lock(&READ_LOCK);
        if (NUM_THREADS_READ == (NUM_THREADS - 1))
        {
          loop = 0;
        }
        pthread_mutex_unlock(&READ_LOCK);
      }

      if (stall == 0)
      {
        pipeline[0].instr = program[PC / 4];
        pipeline[0].pc = PC;
        PC = PC + 4;
        printf("PC - %d\n", PC);
      }

      loop = 1;
      while (loop)
      {
        usleep(DELAY);
        pthread_mutex_lock(&WRITE_LOCK);
        if (NUM_THREADS_WRITE == (NUM_THREADS - 1))
        {
          NUM_THREADS_WRITE = 0;
          NUM_THREADS_READ = 0;
          loop = 0;
        }
        pthread_mutex_unlock(&WRITE_LOCK);
      }

      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 0;
      pthread_mutex_unlock(&CLOCK_LOCK);

      // Implement READ_CLOCK_0 ?
    }
    usleep(DELAY);
  }
}