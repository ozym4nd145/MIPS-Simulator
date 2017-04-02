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
    if (STOP_THREAD == 1)
    {
      pipeline[3].instr.Itype = NO_OP;
      pipeline[3].instr.Ctype = NO_OP;
      temp_pipeline[3].instr.Itype = NO_OP;
      temp_pipeline[3].instr.Ctype = NO_OPERATION;
      CURR_INSTR[4] = pipeline[3].instr;
      ACTIVE_STAGE[4] = 0;
#ifdef DEBUG
      printf("register_write thread ended\n");
#endif
      regdump();
      break;
    }
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
      temp_pipeline[3] = pipeline[3];
      // Signal that was read
      CURR_INSTR[4] = pipeline[3].instr;
#ifdef DEBUG
      instruction_to_file("results/5_register_write.txt", temp_pipeline[3]);
#endif

      // setting default display signal
      ACTIVE_STAGE[4] = 1;

      // If the instruction is not No Operation
      if (pipeline[3].instr.Itype != NO_OP)
      {
        INSTRUCTION_COUNT++;
#ifdef DEBUG
        printf("%s\n", get_instruction_name(temp_pipeline[3].instr.Itype));
        printf("Instruction Count %d\n", INSTRUCTION_COUNT);

#endif

        // Write value into the Register file
        if (pipeline[3].instr.Itype == LDR_BYTE ||
            pipeline[3].instr.Itype == LDR_WORD ||
            pipeline[3].instr.Itype == LDR_UPPER_IMMEDIATE)
        {
          register_file[pipeline[3].instr.rt] = pipeline[3].rt_val;
          CONTROL_SIGN.RegW = 1;
        }
        else if (pipeline[3].instr.Itype == MULTIPLY ||
                 pipeline[3].instr.Itype == MULTIPLY_ADD)
        {
          register_file[32] = pipeline[3].LO;
          register_file[33] = pipeline[3].HI;
          CONTROL_SIGN.RegW = 1;
        }
        else if (pipeline[3].instr.Ctype == DP)
        {
          register_file[pipeline[3].instr.rd] = pipeline[3].alu_result;
          CONTROL_SIGN.RegW = 1;
        }
      }
      else
      {
        ACTIVE_STAGE[4] = 0;
        // do nothing
      }

      // NOTE: we change the values of register file in the reading stage itself
      // as the register file has to updated in the same clock cycle

      // updating that this thread has completed reading stage
      pthread_mutex_lock(&READ_LOCK);
      NUM_THREADS_READ++;
      // printf("RW - Increased NUMREAD - %d\n", NUM_THREADS_READ);
      pthread_mutex_unlock(&READ_LOCK);

      // Updating that processing stage is complete (NO processing needed as
      // values were already updated)
      pthread_mutex_lock(&WRITE_LOCK);
      NUM_THREADS_WRITE++;
      pthread_mutex_unlock(&WRITE_LOCK);

      // Indicates that this instruction is completed and not to again run loop
      // for same instruction
      new_instruction = 0;
#ifdef DEBUG
      print_registers("results/5_register_write.txt");
#endif
    }

    // Adding delay before checking for new instruction
    usleep(DELAY);
  }
  pthread_exit(NULL);
}
