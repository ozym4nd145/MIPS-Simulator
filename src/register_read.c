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
    if(read)
    {
    temp_pipeline[0]=pipeline[0];
    }
    else
    {
    if (temp_pipeline[0].instr.type != NO_OP)
    {
      // Reading stage
      if ((pipeline[1].instr.Itype == LDR_BYTE ||
           pipeline[1].instr.Itype == LDR_WORD) &&
          (pipeline[1].instr.rt == temp_pipeline[0].instr.rs ||
           pipeline[1].instr.rt == temp_pipeline[0].instr.rt))
      {
        control_signal.stall = 1;
      }
      else
      {
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
  }
    sleep(1);
  }
}