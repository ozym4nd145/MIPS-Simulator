#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

/**
* @description: Reads the values of register indicated from opcode into the next
*               pipeline
**/
void* register_read(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;

  while (1)
  {
    if (STOP_THREAD == 1)
    {
      pipeline[0].instr.Itype = NO_OP;
      pipeline[0].instr.Ctype = NO_OP;
      temp_pipeline[0].instr.Itype = NO_OP;
      temp_pipeline[0].instr.Ctype = NO_OPERATION;
      ACTIVE_STAGE[1] = 0;
#ifdef DEBUG
      printf("Register Read thread Stopped\n");
#endif
      break;
    }
    // NOTE: does reading really require lock?

    // wait for the new instruction to occur
    pthread_mutex_lock(&CLOCK_LOCK);
    if (CLOCK == 1)
    {
      clock_start = 1;
    }
    if (CLOCK == 0)
    {
      // indicates that the current instruction has ended
      new_instruction = 1;
      clock_start = 0;
    }
    pthread_mutex_unlock(&CLOCK_LOCK);

    if (clock_start && new_instruction)
    {
      // copy previous pipeline : Reading stage
      temp_pipeline[0] = pipeline[0];
      // Signal that was read
      CURR_INSTR[1] = pipeline[0].instr;
#ifdef DEBUG
      instruction_to_file("results/2_register_read.txt", temp_pipeline[0]);
#endif

      // Setting stall signal to 1 if necessary
      if (temp_pipeline[0].instr.Itype != NO_OP)
      {
        // Stalling cases
        if ((pipeline[1].instr.Itype == LDR_BYTE ||
             pipeline[1].instr.Itype == LDR_WORD) &&
            (pipeline[1].instr.rt == temp_pipeline[0].instr.rs ||
             pipeline[1].instr.rt == temp_pipeline[0].instr.rt))
        {
          control_signal.stall = 1;
          STALL_COUNT++;
        }
        else
        {
          control_signal.stall = 0;
        }

        // setting display
        ACTIVE_STAGE[1] = 1;
      }
      else
      {
        control_signal.stall = 0;
        // setting display
        ACTIVE_STAGE[1] = 0;
      }

      // update that this thread has  completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      // printf("RR - Increased NUMREAD - %d\n", NUM_THREADS_READ);
      pthread_mutex_unlock(&READ_LOCK);

      // wait for all the threads to complete reading
      while (1)
      {
        usleep(DELAY);
        pthread_mutex_lock(&READ_LOCK);
        if (NUM_THREADS_READ == (NUM_THREADS - 1))
        {
          pthread_mutex_unlock(&READ_LOCK);
          break;
        }
        pthread_mutex_unlock(&READ_LOCK);
      }

      // Process instruction if its not NO_OP
      if (temp_pipeline[0].instr.Itype != NO_OP)
      {
        // Stalling cases
        if ((pipeline[1].instr.Itype == LDR_BYTE ||
             pipeline[1].instr.Itype == LDR_WORD) &&
            (pipeline[1].instr.rt == temp_pipeline[0].instr.rs ||
             pipeline[1].instr.rt == temp_pipeline[0].instr.rt))
        {
          // as PC value was incremented in read stage by instruction_fetch.
          PC -= 4;
          // INSTRUCTION_COUNT--;
          pipeline[1].instr.Itype = NO_OP;
          pipeline[1].instr.Ctype = NO_OPERATION;
        }
        // else write values of register into next pipeline
        else
        {
          pipeline[1].instr = temp_pipeline[0].instr;
          pipeline[1].pc = temp_pipeline[0].pc;
          pipeline[1].rs_val = register_file[temp_pipeline[0].instr.rs];
          pipeline[1].rt_val = register_file[temp_pipeline[0].instr.rt];
          pipeline[1].rd_val = register_file[temp_pipeline[0].instr.rd];
          pipeline[1].LO = register_file[32];
          pipeline[1].HI = register_file[33];
        }
      }
      else
      {
        // if NO_OP then just propagate the instruction
        pipeline[1] = temp_pipeline[0];
      }

      // update that this thread has completed processing
      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
#ifdef DEBUG
      instruction_to_file("results/2_register_read.txt", pipeline[1]);
#endif
    }

    // Adding delay before checking for new instruction
    usleep(DELAY);
  }
  pthread_exit(NULL);
}
