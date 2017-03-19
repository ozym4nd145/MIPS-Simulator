#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* register_read(void* data)
{
  printf("Inside Register Read");
  while (1)
  {
    if (pipeline[0].instr.type != NO_OP)
    {
      // Reading stage
      if ((pipeline[1].instr.Itype == LDR_BYTE ||
           pipeline[1].instr.Itype == LDR_WORD) &&
          (pipeline[1].instr.rt == pipeline[0].instr.rs ||
           pipeline[1].instr.rt == pipeline[0].instr.rt))
      {
        control_signal.stall = 1;
        pipeline[1].instr.Itype = NO_OP;
        pipeline[1].instr.Ctype = NO_OPERATION;
      }
      else
      {
        pipeline[1].instr = pipeline[0].instr;
        pipeline[1].pc = pipeline[0].pc;
        pipeline[1].rs_val = register_file[pipeline[0].instr.rs];
        pipeline[1].rt_val = register_file[pipeline[0].instr.rt];
        pipeline[1].rd_val = register_file[pipeline[0].instr.rd];

        // if last pipeline contains useful variable

        // if the updating instruction was a data processing one
        if (pipeline[3].instr.Itype == DP)
        {
          if (pipeline[3].instr.rd == pipeline[0].instr.rs)
          {
            pipeline[1].rs_val = pipeline[3].alu_result;
          }
          if (pipeline[3].instr.rd == pipeline[0].instr.rt)
          {
            pipeline[1].rt_val = pipeline[3].alu_result;
          }
          if (pipeline[3].instr.rd == pipeline[0].instr.rd)
          {
            pipeline[1].rd_val = pipeline[3].alu_result;
          }
        }
        // if it was a Loading instruction
        else if (pipeline[3].instr.Itype == LDR_WORD ||
                 pipeline[3].instr.Itype == LDR_BYTE)
        {
          if (pipeline[3].instr.rt == pipeline[0].instr.rs)
          {
            pipeline[1].rs_val = pipeline[3].rl_val;
          }
          if (pipeline[3].instr.rt == pipeline[0].instr.rt)
          {
            pipeline[1].rt_val = pipeline[3].rl_val;
          }
          if (pipeline[3].instr.rt == pipeline[0].instr.rd)
          {
            pipeline[1].rd_val = pipeline[3].rl_val;
          }
        }
      }
    }
    else
    {
      pipeline[1] = pipeline[0];
    }
    sleep(1);
  }
}