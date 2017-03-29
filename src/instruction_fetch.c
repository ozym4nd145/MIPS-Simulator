#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

/**
* @description: Fetches the appropriate instruction from instruction memory
*               and updates PC accordingly
*/
void* instruction_fetch(void* data)
{
  // printf("Inside Instruction fetch\n");
  char input[100];
  input[0] = 's';
  input[1] = 't';
  input[2] = 'e';
  input[3] = 'p';
  input[4] = '\0';
  int temp_pc;

  while (1)
  {
    scanf("%s", input);
    // sleep(1);
    // input = "step\0";
    if (strcmp(input, "step") == 0)
    {
      // if (PC >= MAX_PC)
      // {
      //   // printf("Program complete");
      //   // TODO: Close all threads and free all memory
      //   // exit(0);
      // }
      STEPS++;

      // lock CLOCK for updating
      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 1;
      pthread_mutex_unlock(&CLOCK_LOCK);

      int stall = control_signal.stall;
      printf("Step = %d | Stall = %d\n", STEPS, stall);
      temp_pc = PC;

      // Setting display
      ACTIVE_STAGE[0] = 1;

      // update value of pc( not a problem in stalls as register_read
      // automatically decrements pc)
      PC += 4;
      

      if(PC >= MAX_PC + 6*4)
      {
        STOP_THREAD=1;
        printf("Instruction Thread Ended\n");
        break;
      }

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
      }

      if (control_signal.stall == 0)
      {
        if (temp_pc > MAX_PC)
        {
          printf("Reached greater than PC\n");
          pipeline[0].instr.Itype = NO_OP;
          pipeline[0].instr.Ctype = NO_OPERATION;
          // printf("Program complete");
          // TODO: Close all threads and free all memory
          // exit(0);
        }
        else
        {
          INSTRUCTION_COUNT++;
          printf("Instruction Count %d\n",INSTRUCTION_COUNT );
          pipeline[0].instr = program[(temp_pc - BASE_PC_ADDR) / 4];
        }
        pipeline[0].pc = temp_pc;

      }
      else
      {
        ACTIVE_STAGE[0] = 0;
      }
      // wait for the rest of the threads to complete write stage
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

      if (control_signal.branched == 1)
      {
        // if(pipeline[0].instr.Itype!=NO_OP)
        //   INSTRUCTION_COUNT--;
        pipeline[0].instr.Itype = NO_OP;
        pipeline[0].instr.Ctype = NO_OPERATION;
        // if(pipeline[1].instr.Itype!=NO_OP)
        //   INSTRUCTION_COUNT--;
        pipeline[1].instr.Itype = NO_OP;
        pipeline[1].instr.Ctype = NO_OPERATION;
        control_signal.branched = 0;
      }

      printf("PC - %08x\n", temp_pc);
      instruction_to_file("results/1_instruction_fetch.txt", pipeline[0]);
      // make clock 0 thus marking the end of the instruction
      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 0;
      pthread_mutex_unlock(&CLOCK_LOCK);

      // Implement READ_CLOCK_0 ?
    }
    else if (strcmp(input, "regdump") == 0)
    {
      regdump();
    }
    else if (strcmp(input, "memdump") == 0)
    {
      int start, num;
      scanf(" 0x%x %d", &start, &num);
      memdump(start, num);
    }
    usleep(DELAY);
  }
  pthread_exit(NULL);
}