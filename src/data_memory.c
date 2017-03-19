#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* memory_op(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;

  while (1)
  {
    // does reading really require lock?
    pthread_mutex_lock(&CLOCK_LOCK, NULL);
    if (CLOCK == 1)
    {
      clock_start = 1;
    }
    if (CLOCK == 0)
    {
      new_instruction = 1;
    }
    pthread_mutex_unlock(&CLOCK_LOCK, NULL);

    if (clock_start && new_instruction)
    {
      temp_pipeline[2] = pipeline[2];

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK, NULL);
      NUM_THREADS_READ++;
      pthread_mutex_unlock(&READ_LOCK, NULL);

      int loop = 1;
      while (loop)
      {
        usleep(DELAY);
        pthread_mutex_lock(&READ_LOCK, NULL);
        if (NUM_THREADS_READ == (NUM_THREADS - 1))
        {
          loop = 0;
        }
        pthread_mutex_unlock(&READ_LOCK, NULL);
      }

      pipeline[3] = temp_pipeline[2];
      if (temp_pipeline[2].instr.Itype == NO_OP)
      {
        // sleep;
      }
      if (temp_pipeline[2].instr.Ctype == DT)
      {
        int write_val = temp_pipeline[2].rt_val;
        int offset = temp_pipeline[2].alu_result;

        if ((temp_pipeline[3].instr.Itype == LDR_WRD ||
             temp_pipeline[3].instr.Itype == LDR_BYTE) &&
            temp_pipeline[3].instr.rt == temp_pipeline[2].instr.rt)
        {
          write_val = temp_pipeline[3].rt_val;
        }
        else if ((temp_pipeline[3].instr.Ctype == DP) &&
                 temp_pipeline[3].instr.rd == temp_pipeline[2].instr.rt)
        {
          write_val = temp_pipeline[3].rd_val;
        }

        switch (temp_pipeline[2].Itype)
        {
          case LDR_WRD:
          {
            pipeline[3].rt_val = Memory_Block[offset / 4];
            break;
          }
          case LDR_BYTE:
          {
            pipeline[3].rt_val = Memory_Block[offset / 4];
            break;
          }
          case STR_WRD:
          {
            Memory_Block[offset / 4] = write_val;
            break;
          }
          case STR_BYTE:
          {
            Memory_Block[offset / 4] = write_val;
            break;
          }
        }
      }

      pthread_mutex_lock(&WRITE_LOCK, NULL);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK, NULL);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
    }
    usleep(DELAY);
  }
}
