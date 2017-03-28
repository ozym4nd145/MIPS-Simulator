#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

/**
* @description: Performs Arithmetic operations on the input based upon opcode.
*/
void* alu_op(void* data)
{
  int clock_start = 0;
  int new_instruction = 1;
  long long int temp;
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
      temp_pipeline[1] = pipeline[1];
      instruction_to_file("results/3_alu_thread.txt", temp_pipeline[1]);

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      // printf("ALU - Increased NUMREAD - %d\n", NUM_THREADS_READ);
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

      // Forwading in case of Multiply instruction
      if (temp_pipeline[2].instr.Itype == MULTIPLY &&
          temp_pipeline[2].instr.Itype == MULTIPLY_ADD)
      {
        temp_pipeline[1].HI = temp_pipeline[2].HI;
        temp_pipeline[1].LO = temp_pipeline[2].LO;
      }
      else if (temp_pipeline[3].instr.Itype == MULTIPLY &&
               temp_pipeline[3].instr.Itype == MULTIPLY_ADD)
      {
        temp_pipeline[1].HI = temp_pipeline[3].HI;
        temp_pipeline[1].LO = temp_pipeline[3].LO;
      }

      pipeline[2] = temp_pipeline[1];
      int r1 = temp_pipeline[1].rs_val;
      int r2 = temp_pipeline[1].rt_val;
      // values loaded for operation to support data forwarding

      // ALU to ALU Data Forwarding (Path1)

      if (temp_pipeline[1].instr.rs == temp_pipeline[2].instr.rd &&
          temp_pipeline[2].instr.Ctype == DP)
      {
        r1 = temp_pipeline[2].alu_result;
      }
      // DATA Memory to ALU Data Forwarding (Path 2)
      else if (temp_pipeline[1].instr.rs == temp_pipeline[3].instr.rt &&
               (temp_pipeline[3].instr.Itype == LDR_BYTE ||
                temp_pipeline[3].instr.Itype == LDR_WORD ||
                temp_pipeline[3].instr.Itype == LDR_UPPER_IMMEDIATE))
      {
        r1 = temp_pipeline[3].rt_val;
      }
      else if (temp_pipeline[1].instr.rs == temp_pipeline[3].instr.rd &&
               (temp_pipeline[3].instr.Ctype == DP &&
                (temp_pipeline[3].instr.Itype != MULTIPLY ||
                 temp_pipeline[3].instr.Itype != MULTIPLY_ADD)))
      {
        r1 = temp_pipeline[3].alu_result;
      }
      // Similar check for operand2 of ALU

      if (temp_pipeline[1].instr.rt == temp_pipeline[2].instr.rd &&
          temp_pipeline[2].instr.Ctype == DP)
      {
        r2 = temp_pipeline[2].alu_result;
      }
      else if (temp_pipeline[1].instr.rt == temp_pipeline[3].instr.rt &&
               (temp_pipeline[3].instr.Itype == LDR_BYTE ||
                temp_pipeline[3].instr.Itype == LDR_WORD ||
                temp_pipeline[3].instr.Itype == LDR_UPPER_IMMEDIATE))
      {
        r2 = temp_pipeline[3].rt_val;
      }
      else if (temp_pipeline[1].instr.rt == temp_pipeline[3].instr.rd &&
               (temp_pipeline[3].instr.Ctype == DP &&
                (temp_pipeline[3].instr.Itype != MULTIPLY ||
                 temp_pipeline[3].instr.Itype != MULTIPLY_ADD)))
      {
        r2 = temp_pipeline[3].alu_result;
      }

      // processing instruction to perform ALU operations

      switch (temp_pipeline[1].instr.Ctype)
      {
        case DP:
        {
          switch (temp_pipeline[1].instr.Itype)
          {
            case ADD:
              pipeline[2].alu_result = r1 + r2;
              break;

            case ADDI:
              pipeline[2].alu_result = r1 + temp_pipeline[1].instr.immediate;
              pipeline[2].instr.rd = temp_pipeline[1].instr.rt;
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

            case ORI:
              pipeline[2].alu_result = (r1 | temp_pipeline[1].instr.immediate);
              pipeline[2].instr.rd = temp_pipeline[1].instr.rt;
              break;

            case SLTU:
              if (r1 < r2)
                pipeline[2].alu_result = 1;
              else
                pipeline[2].alu_result = 0;
              break;

            case SLTI:
              if (r1 < temp_pipeline[1].instr.immediate)
                pipeline[2].alu_result = 1;
              else
                pipeline[2].alu_result = 0;

              pipeline[2].instr.rd = temp_pipeline[1].instr.rt;
              break;

            case LOGIC_SHIFT_LEFT:
              pipeline[2].alu_result = (r2 << (temp_pipeline[1].instr.shf_amt));
              break;

            case AND:
              pipeline[2].alu_result = r1 & r2;
              break;

            case LOGIC_SHIFT_LEFT_VARIABLE:
              pipeline[2].alu_result =
                  r2 << (r1 &
                         (0x0000001F));  // looking at last 5 bits of register
              break;

            // HI LO are 2 separate registers for multiplication

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
              pipeline[2].alu_result = temp_pipeline[1].instr.immediate + r1;
              break;

            case LDR_UPPER_IMMEDIATE:
              pipeline[2].alu_result = (temp_pipeline[1].instr.immediate << 16);
              break;
            // Similar ALU operation to be performed for all DT instruction
            default:
              throw_error("Unrecognized Instruction");
          }
          break;
        }
        // Branch Class Cases Handling and Updating PC if branch occurs

        case BRANCH:
        {
          int branched = 0;
          switch (temp_pipeline[1].instr.Itype)
          {
            case BRANCH_EQUAL:
              if (r1 == r2)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_GREATER_OR_EQUAL:
              if (r1 >= 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_LESS_OR_EQUAL:
              if (r1 <= 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_GREATER:
              if (r1 > 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            case BRANCH_LESS:
              if (r1 < 0)
              {
                PC += temp_pipeline[1].pc +
                      (temp_pipeline[1].instr.immediate << 2);
                branched = 1;
              }
              break;

            default:
              throw_error("Wrong Instruction");
          }

          // If branch is successful filling bubbles/no-operation in Pipeline
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
      // updating next pipeline buffer (required if Data Forwading had occured)

      // update that this thread has completed processing
      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
      instruction_to_file("results/3_alu_thread.txt", pipeline[2]);
    }

    // Adding delay before checking for new instruction
    usleep(DELAY);
  }
}
