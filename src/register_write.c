#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* register_write(void* data)
{
  printf("Inside Register Write");
  // Running it all the time
  while (1)
  {
    // If the instruction is not No Operation
    if (pipeline[3].instr.type != NO_OP)
    {
      // Write value into the Register file
      if (pipeline[3].instr.Itype == LDR_BYTE ||
          pipeline[3].instr.Itype == LDR_WORD)
      {
        register_file[pipeline[3].instr.rt] = pipeline[3].rt_val;
      }
      else if (pipeline[3].instr.Ctype == DP)
      {
        register_file[pipeline[3].instr.rd] = pipeline[3].alu_result;
      }
    }
    else
    {
      // do nothing
    }
    sleep(1);
  }
}