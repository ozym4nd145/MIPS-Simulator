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
  int program_end = 0;

  while (1)
  {

    if (program_end)
      break;


    if (scanf("%s", input) == -1)
    {
      continue;
      sleep(DELAY);
    }

#ifdef TIME
    double time_spent;
    struct timespec begin;
    struct timespec end;
#endif

    // sleep(1);
    // input = "step\0";
    if (strcmp(input, "run") == 0 || strcmp(input, "continue") == 0)
    {

      int run = 0;

      if (strcmp(input, "run") == 0)
        run=1;

      while(1)
      {
      STEPS++;

#ifdef TIME
      clock_gettime(CLOCK_REALTIME, &begin);
#endif
      if (PC >= MAX_PC + 5 * 4)
      {
        STOP_THREAD = 1;
        ACTIVE_STAGE[0] = 0;
#ifdef DEBUG
        printf("Instruction Thread Ended\n");
#endif
        program_end = 1;
        break;
      }


      CONTROL_SIGN.MemWr = 0;
      CONTROL_SIGN.MemRd = 0;
      CONTROL_SIGN.FWD_ALU = 0;
      CONTROL_SIGN.FWD_DM = 0;
      CONTROL_SIGN.TO_ALU = 0;
      CONTROL_SIGN.TO_DM = 0;
      CONTROL_SIGN.M2R = 0;
      CONTROL_SIGN.FLUSH = 0;
      CONTROL_SIGN.PCsrc = 0;
      CONTROL_SIGN.RegW = 0;
      CONTROL_SIGN.STALL_C = 0;

      // lock CLOCK for updating

      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 1;
      pthread_mutex_unlock(&CLOCK_LOCK);

      int stall = control_signal.stall;
      printf("Step = %d\n", STEPS);

      if(PC<BASE_PC_ADDR)
      {
        throw_error("Wrong Branch Address");
      }

      temp_pc = PC;

      // Setting display
      // ACTIVE_STAGE[0] = 1;

      // update value of pc( not a problem in stalls as register_read
      // automatically decrements pc)
      PC += 4;

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
      //   if(control_signal.stall == 1)
      // {
      //   pipeline[0].instr = program[(temp_pc - BASE_PC_ADDR) / 4];
      //   CURR_INSTR[0] = program[(temp_pc - BASE_PC_ADDR) / 4];
      // }

      if (stall == 0)
      {
        if (temp_pc > MAX_PC)
        {
          ACTIVE_STAGE[0] = 0;
#ifdef DEBUG
          printf("Reached greater than PC\n");
#endif
          pipeline[0].instr.Itype = NO_OP;
          pipeline[0].instr.Ctype = NO_OPERATION;
          CURR_INSTR[0].Itype = NO_OP;
          CURR_INSTR[0].Ctype = NO_OPERATION;
        }
        else
        {
          ACTIVE_STAGE[0] = 1;
#ifdef DEBUG
//          printf("Instruction Count %d\n", INSTRUCTION_COUNT);
#endif
          if (control_signal.stall == 0)
          {
            pipeline[0].instr = program[(temp_pc - BASE_PC_ADDR) / 4];
            CURR_INSTR[0] = program[(temp_pc - BASE_PC_ADDR) / 4];
          }
          else
          {
            pipeline[0].instr = program[(temp_pc - BASE_PC_ADDR) / 4 - 1];
            CURR_INSTR[0] = program[(temp_pc - BASE_PC_ADDR) / 4];
          }
          INSTRUCTION_MEM_ACCESS++;
        }
        if (control_signal.stall == 0)
          pipeline[0].pc = temp_pc;
        else
          pipeline[0].pc = temp_pc - 4;
#ifdef DEBUG
        printf("Instruction Mem Access %d %s\n", INSTRUCTION_MEM_ACCESS,
               get_instruction_name(CURR_INSTR[0].Itype));
#endif
      }
      else
      {
        pipeline[0].instr = program[(temp_pc - BASE_PC_ADDR) / 4];
        CURR_INSTR[0] = program[(temp_pc - BASE_PC_ADDR) / 4];
        pipeline[0].pc = temp_pc;
        CONTROL_SIGN.STALL_C = 1;
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
        CONTROL_SIGN.FLUSH = 1;
        CONTROL_SIGN.PCsrc = 1;
        if (pipeline[0].instr.Itype != NO_OP)
        {
          // INSTRUCTION_COUNT--;
          BRANCH_CYCLE_WASTE++;
        }
        pipeline[0].instr.Itype = NO_OP;
        pipeline[0].instr.Ctype = NO_OPERATION;
        if (pipeline[1].instr.Itype != NO_OP)
        {
          // INSTRUCTION_COUNT--;
          BRANCH_CYCLE_WASTE++;
        }
        pipeline[1].instr.Itype = NO_OP;
        pipeline[1].instr.Ctype = NO_OPERATION;
        control_signal.branched = 0;
      }

#ifdef DEBUG
      printf("PC - %08x\n", temp_pc);
      printf("BRANCH_CYCLE_WASTE %d\n", BRANCH_CYCLE_WASTE);
      instruction_to_file("results/1_instruction_fetch.txt", pipeline[0]);
#endif
      // make clock 0 thus marking the end of the instruction
      pthread_mutex_lock(&CLOCK_LOCK);
      CLOCK = 0;
      pthread_mutex_unlock(&CLOCK_LOCK);

#ifdef TIME
      clock_gettime(CLOCK_REALTIME, &end);
      time_spent = (double)((end.tv_sec - begin.tv_sec) * (1e9) +
                            (double)(end.tv_nsec - begin.tv_nsec));
      printf("Half cycle time - %lf\n", time_spent);
      printf("Time Period - %lf\n", 2 * time_spent);
      printf("Frequency - %lfGHz\n", 1.0 / (2 * time_spent));
#endif

      usleep(10*DELAY);

      // Implement READ_CLOCK_0 ?
      if(BreakPoint[PC]==1 && !run)
        stall=1;

      if(BreakPoint[PC]==1 && run)
      {
        if(pipeline[0].instr.Itype==NO_OP && pipeline[1].instr.Itype == NO_OP && pipeline[2].instr.Itype == NO_OP 
          && pipeline[3].instr.Itype == NO_OP)
          break;
      }
    } // continue stepping instructions loop ends
  } // run | continue if ends


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


    else if (strcmp(input, "delete") == 0)
    {
      int break_address;
      scanf(" 0x%x", &break_address);
      int index = (break_address-BASE_PC_ADDR)/4;

      if(index<0 || index > INSTRUCTION_MEM )
        printf("Invalid BreakPoint Address\n" );

      else if ( BreakPoint[index]==0 )
        printf("BreakPoint does not exist at specified address\n");

      else
        BreakPoint[index]=0;
    }


    else if (strcmp(input, "break") == 0)
    {
      int break_address;
      scanf(" 0x%x", &break_address);
      int index = (break_address-BASE_PC_ADDR)/4;

      if(index<0 || index > INSTRUCTION_MEM )
        printf("Invalid BreakPoint Address\n" );

      else if ( BreakPoint[index]==1 )
        printf("BreakPoint already exists at specified address\n");

      else
        BreakPoint[index]=1;
    }


    else
    {
      printf("Unrecognized command\n");
    }
    
    usleep(DELAY);
  }
  pthread_exit(NULL);
}
