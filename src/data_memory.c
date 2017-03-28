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
      // copy previous pipeline : Reading stage
      temp_pipeline[2] = pipeline[2];
      instruction_to_file("results/4_data_memory_thread.txt", temp_pipeline[2]);
      // printing for debugging

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      // printf("DM - Increased NUMREAD - %d\n", NUM_THREADS_READ);
      pthread_mutex_unlock(&READ_LOCK);

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
        // sleep;
      }
      if (temp_pipeline[2].instr.Ctype == DT)
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
          write_val = temp_pipeline[3].rt_val;
        }
        // Data forwarding from exit of Data Memory to input of Data Memory for
        // ALU operation

        else if ((temp_pipeline[3].instr.Ctype == DP) &&
                 temp_pipeline[3].instr.rd == temp_pipeline[2].instr.rt)
        {
          write_val = temp_pipeline[3].rd_val;
        }

        switch (temp_pipeline[2].instr.Itype)
        {
          case LDR_WORD:
          {
            pipeline[3].rt_val = Memory_Block[(offset - BASE_ADDR) / 4];
            break;
          }
          case LDR_BYTE:
          {
          	int x=(offset - BASE_ADDR) % 4;
          	int y= Memory_Block[(offset - BASE_ADDR) / 4];
          	int z;

          	switch(x)
          	{
          		case 0:
          		z=(y>>24);
          		break;
          		case 1:
          		z=(y<<8)>>24;
          		break;
          		case 2:
          		z=(y<<16)>>24;
          		break;
          		case 3:
          		z=(y<<24)>>24;
          		break;
          	}


            pipeline[3].rt_val =z ;
            break;
          }
          case STR_WORD:
          {
            Memory_Block[(offset - BASE_ADDR) / 4] = write_val;
            break;
          }
          case STR_BYTE:
          {
          	int byte_pos=(offset - BASE_ADDR) % 4;
          	int index=(offset - BASE_ADDR) / 4;
          	int write_val2=write_val & (0x0000FF);
          	int temp=Memory_Block[index];

          	switch (byte_pos)
          	{
          		case 0:
          		temp=(temp&0x00FFFFFF)&(write_val2<<24);
          		break;
          		case 1:
          		temp=(temp&0xFF00FFFF)&(write_val2<<16);
          		break;
          		case 2:
          		temp=(temp&0xFFFF00FF)&(write_val2<<8);
          		break;
          		case 3:
          		temp=(temp&0xFFFFFF00)&(write_val2);
          		break;
          	}
          	Memory_Block[index]=temp;
            
            break;
          }
          case LDR_UPPER_IMMEDIATE:
          {
            pipeline[3].rt_val = temp_pipeline[2].alu_result;
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
      instruction_to_file("results/4_data_memory_thread.txt", pipeline[3]);
    }

    // Adding delay before checking for new instruction
    usleep(DELAY);
  }
}
