#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* memory_op(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;
  CLOCK_ZERO_READ[2] = 1;

  while (1)
  {
    if (STOP_THREAD == 1)
    {
      pipeline[2].instr.Itype = NO_OP;
      pipeline[2].instr.Ctype = NO_OP;
      temp_pipeline[2].instr.Itype = NO_OP;
      temp_pipeline[2].instr.Ctype = NO_OPERATION;
      ACTIVE_STAGE[3] = 0;
      CONTROL_SIGN.FWD_DM = 0;
      CONTROL_SIGN.TO_DM = 0;

#ifdef DEBUG
      printf("Memory Thread Stopped\n");
#endif

      break;
    }
    // does reading really require lock?

    // wait for the new instruction to occur
    pthread_mutex_lock(&CLOCK_LOCK);
    if (CLOCK == 1)
    {
      clock_start = 1;
      CLOCK_ZERO_READ[2] = 0;
    }
    if (CLOCK == 0)
    {
      // indicates that the current instruction has ended
      clock_start = 0;
      new_instruction = 1;
      CLOCK_ZERO_READ[2] = 1;
    }
    pthread_mutex_unlock(&CLOCK_LOCK);

    if (clock_start && new_instruction)
    {
      // copy previous pipeline : Reading stage
      temp_pipeline[2] = pipeline[2];
      // Signal that was read
      CURR_INSTR[3] = pipeline[2].instr;
#ifdef DEBUG
      instruction_to_file("results/4_data_memory_thread.txt", temp_pipeline[2]);
#endif

      // printing for debugging

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      // printf("DM - Increased NUMREAD - %d\n", NUM_THREADS_READ);
      pthread_mutex_unlock(&READ_LOCK);

      // setting default display signal
      ACTIVE_STAGE[3] = 1;
      CONTROL_SIGN.FWD_DM = 0;
      CONTROL_SIGN.TO_DM = 0;

      // wait for all the threads to complete reading
      while (1)
      {
        usleep(DELAY);
        // sleep to prevent locking
        pthread_mutex_lock(&READ_LOCK);
        if (NUM_THREADS_READ == (NUM_THREADS - 1))
        {
          pthread_mutex_unlock(&READ_LOCK);
          break;
        }
        pthread_mutex_unlock(&READ_LOCK);
      }

      pipeline[3] = temp_pipeline[2];
      // copying the whole structure into a temporary buffer pipeline in reading
      // stage

      if (temp_pipeline[2].instr.Itype == NO_OP)
      {
        ACTIVE_STAGE[3] = 0;
        CONTROL_SIGN.FWD_DM = 0;
        CONTROL_SIGN.TO_DM = 0;
        // sleep;
      }
      else if (temp_pipeline[2].instr.Ctype == DT)
      {
        // instruction of class Data Transfer
        int write_val = temp_pipeline[2].rt_val;
        int offset = temp_pipeline[2].alu_result;
        // Data forwarding from exit of Data Memory to input of Data Memory

        if ((temp_pipeline[3].instr.Itype == LDR_WORD ||
             temp_pipeline[3].instr.Itype == LDR_BYTE ||
             temp_pipeline[3].instr.Itype == LDR_UPPER_IMMEDIATE) &&
            temp_pipeline[3].instr.rt == temp_pipeline[2].instr.rt)
        {
          CONTROL_SIGN.FWD_DM = 1;
          CONTROL_SIGN.TO_DM = 1;
          write_val = temp_pipeline[3].rt_val;
        }
        // Data forwarding from exit of Data Memory to input of Data Memory for
        // ALU operation

        else if ((temp_pipeline[3].instr.Ctype == DP) &&
                 temp_pipeline[3].instr.rd == temp_pipeline[2].instr.rt)
        {
          CONTROL_SIGN.FWD_DM = 1;
          CONTROL_SIGN.TO_DM = 1;
          write_val = temp_pipeline[3].alu_result;
        }

        switch (temp_pipeline[2].instr.Itype)
        {
          case LDR_WORD:
          {
            // pipeline[3].rt_val = Memory_Block[(offset - BASE_ADDR) / 4];
            pipeline[3].rt_val = program_memory_interface(0, offset, 1);
            DATA_MEM_ACCESS++;
            CONTROL_SIGN.MemRd = 1;
            break;
          }
          case LDR_BYTE:
          {
            int x = (offset - BASE_ADDR) % 4;
            // int y = Memory_Block[(offset - BASE_ADDR) / 4];
            int y = program_memory_interface(0, offset, 1);
            DATA_MEM_ACCESS++;
            CONTROL_SIGN.MemRd = 1;
            int z;

            switch (x)
            {
              case 0:
                z = (y >> 24);
                break;
              case 1:
                z = (y << 8) >> 24;
                break;
              case 2:
                z = (y << 16) >> 24;
                break;
              case 3:
                z = (y << 24) >> 24;
                break;
            }

            pipeline[3].rt_val = z;
            break;
          }
          case STR_WORD:
          {
            // printf(
            //     "Inside store word.\nOffset - %08x\nBase - %08x\nWrite -
            //     %d\n",
            //     offset, BASE_ADDR, write_val);
            // Memory_Block[(offset - BASE_ADDR) / 4] = write_val;
            program_memory_interface(write_val, offset, 2);
            DATA_MEM_ACCESS++;
            CONTROL_SIGN.MemWr = 1;
            break;
          }
          case STR_BYTE:
          {
            int byte_pos = (offset - BASE_ADDR) % 4;
            // int index = (offset - BASE_ADDR) / 4;
            int write_val2 = write_val & (0x0000FF);
            // int temp = Memory_Block[index];
            int temp = program_memory_interface(0, offset, 1);
            DATA_MEM_ACCESS++;
            CONTROL_SIGN.MemWr = 1;

            switch (byte_pos)
            {
              case 0:
                temp = (temp & 0x00FFFFFF) & (write_val2 << 24);
                break;
              case 1:
                temp = (temp & 0xFF00FFFF) & (write_val2 << 16);
                break;
              case 2:
                temp = (temp & 0xFFFF00FF) & (write_val2 << 8);
                break;
              case 3:
                temp = (temp & 0xFFFFFF00) & (write_val2);
                break;
            }
            // Memory_Block[index] = temp;
            program_memory_interface(temp, offset, 2);

            break;
          }
          case LDR_UPPER_IMMEDIATE:
          {
            pipeline[3].rt_val = temp_pipeline[2].alu_result;
            // CONTROL_SIGN.MemRd = 1;
            break;
          }
          default:
          {
            throw_error("Error in parsing instructions");
          }
        }
      }

      // update that this thread has completed processing
      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
#ifdef DEBUG
      instruction_to_file("results/4_data_memory_thread.txt", pipeline[3]);
#endif
    }

    // Adding delay before checking for new instruction
    usleep(DELAY);
  }
  pthread_exit(NULL);
}
