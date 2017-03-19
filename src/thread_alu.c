#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* alu_op(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;
  long long int temp;
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
      temp_pipeline[1] = pipeline[1];
      instruction_to_file("results/alu_thread.txt", temp_pipeline[1]);
      printf("%d\n", NUM_THREADS_READ);  //*************************************

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      pthread_mutex_unlock(&READ_LOCK);

      //       FILE *opener;
      // opener=fopen("random.txt","a");

      while (1)
      {
        // fprintf(opener,"NUM_THREADS_READ register_read %d\n",NUM_THREADS_READ
        // );

        usleep(DELAY);
        pthread_mutex_lock(&READ_LOCK);
        if (NUM_THREADS_READ == (NUM_THREADS - 1))
        {
          pthread_mutex_unlock(&READ_LOCK);
          break;
        }
        pthread_mutex_unlock(&READ_LOCK);
      }

      pipeline[2] = temp_pipeline[1];
      int r1 = temp_pipeline[1].rs_val;
      int r2 = temp_pipeline[1].rt_val;

      if (temp_pipeline[1].instr.rs == pipeline[2].instr.rd &&
          pipeline[2].instr.Ctype == DP)
      {
        r1 = pipeline[2].alu_result;
      }
      else if (temp_pipeline[1].instr.rs == pipeline[3].instr.rt &&
               pipeline[3].instr.Itype == LDR_WORD)
      {
        r1 = pipeline[3].rt_val;
      }

      if (temp_pipeline[1].instr.rt == pipeline[2].instr.rd &&
          pipeline[2].instr.Ctype == DP)
      {
        r2 = pipeline[2].alu_result;
      }
      else if (temp_pipeline[1].instr.rt == pipeline[3].instr.rt &&
               pipeline[3].instr.Itype == LDR_WORD)
      {
        r2 = pipeline[3].rt_val;
      }

      switch (temp_pipeline[1].instr.Ctype)
      {
        case DP:
        {
          switch (temp_pipeline[1].instr.Itype)
          {
            case ADD:
              pipeline[2].alu_result = r1 + r2;
              break;

            case SUB:
              pipeline[2].alu_result = r1 - r2;
              break;

            case NOR:
              pipeline[2].alu_result = ~(r1 | r2);
              break;

            case OR:
              pipeline[2].alu_result = (r1 | r2);
              break;

            case LOGIC_SHIFT_LEFT:
              pipeline[2].alu_result = (r2 << (temp_pipeline[1].instr.shf_amt));
              break;

            case AND:
              pipeline[2].alu_result = r1 & r2;
              break;

            case LOGIC_SHIFT_LEFT_VARIABLE:
              pipeline[2].alu_result = r2 << (r1 & (0x0000001F));
              break;

            case MULTIPLY:
              temp = ((long long int)r1) * ((long long int)r2);
              pipeline[2].HI = (int)(((unsigned long long)temp) >> 32);
              pipeline[2].LO = (int)temp;
              break;

            case MULTIPLY_ADD:
              temp = ((long long int)r1) * ((long long int)r2);
              pipeline[2].HI += (int)(((unsigned long long)temp) >> 32);
              pipeline[2].LO += (int)temp;
              break;

            default:
              throw_error("Unrecognized Instruction");
          }
          break;
        }

        case DT:
        {
          switch (temp_pipeline[1].instr.Itype)
          {
            case LDR_WORD:
            case LDR_BYTE:
            case STR_WORD:
            case STR_BYTE:
              pipeline[2].alu_result =
                  temp_pipeline[1].instr.immediate + temp_pipeline[1].rs_val;
              break;
            default:
              throw_error("Unrecognized Instruction");
          }
          break;
        }
        // Branch Class Cases Handling
        case BRANCH:
        {
          int branched = 0;
          switch (temp_pipeline[1].instr.Itype)
          {
            case BRANCH_EQUAL:
              if (temp_pipeline[1].rs_val == temp_pipeline[1].rt_val)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_GREATER_OR_EQUAL:
              if (temp_pipeline[1].rs_val >= 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_LESS_OR_EQUAL:
              if (temp_pipeline[1].rs_val <= 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_GREATER:
              if (temp_pipeline[1].rs_val > 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_LESS:
              if (temp_pipeline[1].rs_val < 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            default:
              throw_error("Wrong Instruction");
          }
          if (branched == 1)
          {
            pipeline[3].instr.Itype = NO_OP;
            pipeline[3].instr.Ctype = NO_OPERATION;
          }

          break;
        }

        case NO_OPERATION:
        {
          // Make thread sleep/Wait
          break;
        }

        default:
          throw_error("Wrong Class of Instruction");
      }

      // Copying relevant contents for next pipeline

      pipeline[2].rs_val = r1;
      pipeline[2].rt_val = r2;

      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
      instruction_to_file("results/alu_thread.txt", temp_pipeline[1]);
    }

    usleep(DELAY);
  }
}
