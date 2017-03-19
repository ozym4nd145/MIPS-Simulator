#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* register_write(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;

  while (1)
  {
    // does reading really require lock?
    // wait for the new instruction to occur
    pthread_mutex_lock(&CLOCK_LOCK);
    if (CLOCK == 1)
    {
      clock_start = 1;
    }
    if (CLOCK == 0)
    {
      // indicates that the current instruction has ended
      clock_start = 0;
      new_instruction = 1;
    }
    pthread_mutex_unlock(&CLOCK_LOCK);

    if (clock_start && new_instruction)
    {
      // If the instruction is not No Operation
      if (pipeline[3].instr.Itype != NO_OP)
      {
        // Write value into the Register file
        if (pipeline[3].instr.Itype == LDR_BYTE ||
            pipeline[3].instr.Itype == LDR_WORD)
        {
          register_file[pipeline[3].instr.rt] = pipeline[3].rt_val;
        }
        else if (pipeline[3].instr.Ctype == DP)
        {
          register_file[pipeline[3].instr.rd] = pipeline[3].alu_result;
        }
      }
      else
      {
        // do nothing
      }

      // NOTE: we change the values of register file in the reading stage itself
      // as the register file has to updated in the same clock cycle

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      printf("RW - Increased NUMREAD - %d\n", NUM_THREADS_READ);
      pthread_mutex_unlock(&READ_LOCK);

      // Updating that processing stage is complete (NO processing needed as
      // values were already updated)
      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
      print_registers("results/5_register_write.txt");
    }

    // Adding delay before checking for new instruction
    usleep(DELAY);
  }
}