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
  int temp_pc;

  while (1)
  {
    // printf("Looping CLOCK %d\n",CLOCK);
    scanf("%s", input);
    if (strcmp(input, "step") == 0)
    {
      STEPS++;

      // lock CLOCK for updating
      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 1;
      pthread_mutex_unlock(&CLOCK_LOCK);

      int stall = control_signal.stall;
      temp_pc = PC;
      PC += 4;
      // loop until reading stage has completed
      while (1)
      {
        usleep(DELAY);
        pthread_mutex_lock(&READ_LOCK);
        if (NUM_THREADS_READ >= (NUM_THREADS - 1))
        {
          pthread_mutex_unlock(&READ_LOCK);
          break;
        }
        pthread_mutex_unlock(&READ_LOCK);
        // printf("NUm_THR_READ %d\n",
        //        NUM_THREADS_READ);  //****************************************
      }

      if (stall == 0)
      {
        // /printf("%d\n",PC );
        pipeline[0].instr = program[temp_pc / 4];
        pipeline[0].pc = temp_pc;
        printf("PC - %d\n", temp_pc);
        instruction_to_file("results/1_instruction_fetch.txt", pipeline[0]);
      }

      while (1)
      {
        usleep(DELAY);
        pthread_mutex_lock(&WRITE_LOCK);
        if (NUM_THREADS_WRITE == (NUM_THREADS - 1))
        {
          NUM_THREADS_WRITE = 0;
          NUM_THREADS_READ = 0;
          pthread_mutex_unlock(&WRITE_LOCK);
          break;
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