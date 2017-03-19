#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* register_read(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;

  printf("Inside Register Read");
  while (1)
  {
    // does reading really require lock?
    pthread_mutex_lock(&CLOCK_LOCK);
    if (CLOCK == 1)
    {
      clock_start = 1;
    }
    if (CLOCK == 0)
    {
      new_instruction = 1;
    }
    pthread_mutex_unlock(&CLOCK_LOCK);

    if (clock_start && new_instruction)
    {
      temp_pipeline[0] = pipeline[0];
       instruction_to_file("results/register_read.txt",temp_pipeline[0]);

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      pthread_mutex_unlock(&READ_LOCK);

FILE *opener;
opener=fopen("random.txt","a");
      int loop = 1;
      while (loop)
      {
         fprintf(opener,"NUM_THREADS_READ register_read %d\n",NUM_THREADS_READ );
        usleep(DELAY);
        pthread_mutex_lock(&READ_LOCK);
        if (NUM_THREADS_READ == (NUM_THREADS - 1))
        {
          loop = 0;
        }
        pthread_mutex_unlock(&READ_LOCK);
        printf("NUM_THREADS_READ register_read %d\n",NUM_THREADS_READ );
      }

      control_signal.stall = 0;
      if (temp_pipeline[0].instr.Itype != NO_OP)
      {
        // Reading stage
        if ((pipeline[1].instr.Itype == LDR_BYTE ||
             pipeline[1].instr.Itype == LDR_WORD) &&
            (pipeline[1].instr.rt == temp_pipeline[0].instr.rs ||
             pipeline[1].instr.rt == temp_pipeline[0].instr.rt))
        {
          control_signal.stall = 1;
          pipeline[1].instr.Itype = NO_OP;
          pipeline[1].instr.Ctype = NO_OPERATION;
        }
        else
        {
          // pipeline[1].instr = pipeline[0].instr;
          // pipeline[1].pc = pipeline[0].pc;
          // pipeline[1].rs_val = register_file[pipeline[0].instr.rs];
          // pipeline[1].rt_val = register_file[pipeline[0].instr.rt];
          // pipeline[1].rd_val = register_file[pipeline[0].instr.rd];

          // // if last pipeline contains useful variable

          // // if the updating instruction was a data processing one
          // if (pipeline[3].instr.Itype == DP)
          // {
          //   if (pipeline[3].instr.rd == pipeline[0].instr.rs)
          //   {
          //     pipeline[1].rs_val = pipeline[3].alu_result;
          //   }
          //   if (pipeline[3].instr.rd == pipeline[0].instr.rt)
          //   {
          //     pipeline[1].rt_val = pipeline[3].alu_result;
          //   }
          //   if (pipeline[3].instr.rd == pipeline[0].instr.rd)
          //   {
          //     pipeline[1].rd_val = pipeline[3].alu_result;
          //   }
          // }
          // // if it was a Loading instruction
          // else if (pipeline[3].instr.Itype == LDR_WORD ||
          //          pipeline[3].instr.Itype == LDR_BYTE)
          // {
          //   if (pipeline[3].instr.rt == pipeline[0].instr.rs)
          //   {
          //     pipeline[1].rs_val = pipeline[3].rl_val;
          //   }
          //   if (pipeline[3].instr.rt == pipeline[0].instr.rt)
          //   {
          //     pipeline[1].rt_val = pipeline[3].rl_val;
          //   }
          //   if (pipeline[3].instr.rt == pipeline[0].instr.rd)
          //   {
          //     pipeline[1].rd_val = pipeline[3].rl_val;
          //   }
          // }
          pipeline[1].instr = temp_pipeline[0].instr;
          pipeline[1].pc = temp_pipeline[0].pc;
          pipeline[1].rs_val = register_file[temp_pipeline[0].instr.rs];
          pipeline[1].rt_val = register_file[temp_pipeline[0].instr.rt];
          pipeline[1].rd_val = register_file[temp_pipeline[0].instr.rd];
        }
      }
      else
      {
        pipeline[1] = temp_pipeline[0];
      }

      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
    }
    instruction_to_file("results/register_read.txt",pipeline[1]);

    usleep(DELAY);
  }
}