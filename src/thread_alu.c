#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void alu_op()
{
  while(1)
  {

    if(read)
    {
      temp_pipeline[1]=pipeline[1];
    }
    else
    {
      pipeline[2]=temp_pipeline[1];
  int r1 = temp_pipeline[1].rs_val;
  int r2 = temp_pipeline[1].rt_val;

  if (temp_pipeline[1].instr.rs == pipeline[2].instr.rd &&
      pipeline[2].instr.Ctype == DP)
  {
    r1 = pipeline[2].alu_result;
  }
  else if (temp_pipeline[1].instr.rs == pipeline[3].instr.rt &&
           pipeline[3].instr.Itype == LDR_WRD)
  {
    r1 = pipeline[3].rt_val;
  }

  if (temp_pipeline[1].instr.rt == pipeline[2].instr.rd &&
      pipeline[2].instr.Ctype == DP)
  {
    r2 = pipeline[2].alu_result;
  }
  else if (temp_pipeline[1].instr.rt == pipeline[3].instr.rt &&
           pipeline[3].instr.Itype == LDR_WRD)
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

        case LOGICAL_SHIFT_LEFT:
          pipeline[2].alu_result = (r2 << (temp_pipeline[1].instr.shft_amt));
          break;

        case AND:
          pipeline[2].alu_result = r1 & r2;
          break;

        case LOGIC_SHIFT_LEFT_VARIABLE:
          pipeline[2].alu_result = r2 << (r1 & (0x0000001F));
          break;

        case MULTIPLY:
          long long int temp = ((long long int)r1) * ((long long int)r2);
          pipeline[2].HI = (int)((unsigned temp) >> 32);
          pipeline[2].LO = (int)temp;
          break;

        case MULTIPLY_ADD:
          long long int temp = ((long long int)r1) * ((long long int)r2);
          pipeline[2].HI += (int)((unsigned temp) >> 32);
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
        case LDR_WRD:
        case LDR_BYTE:
        case STR_WRD:
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
      int branched=0;
      switch (temp_pipeline[1].instr.Itype)
      {
        case BRANCH_EQUAL:
          if (temp_pipeline[1].rs_val == temp_pipeline[1].rt_val)
          {
            PC += temp_pipeline[1].pc + (temp_pipeline[1].instr.immediate << 2);
            branched=1;
          }
          break;

        case BRANCH_GREATER_OR_EQUAL:
          if (temp_pipeline[1].rs_val >= 0)
          {
            PC += temp_pipeline[1].pc + (temp_pipeline[1].instr.immediate << 2);
            branched=1;
          }
          break;

        case BRANCH_LESS_OR_EQUAL:
          if (temp_pipeline[1].rs_val <= 0)
          {
            PC += temp_pipeline[1].pc + (temp_pipeline[1].instr.immediate << 2);
            branched=1;
          }
          break;

        case BRANCH_GREATER:
          if (temp_pipeline[1].rs_val > 0)
          {
            PC += temp_pipeline[1].pc + (temp_pipeline[1].instr.immediate << 2);
            branched=1;
          }
          break;

        case BRANCH_LESS:
          if (temp_pipeline[1].rs_val < 0)
          {
            PC += temp_pipeline[1].pc + (temp_pipeline[1].instr.immediate << 2);
            branched=1;
          }
          break;

        default:
          throw_error("Wrong Instruction");
      }
      if(branched==1)
       {
        pipeline[3].instr.Itype=NO_OP;
        pipeline[3].instr.Ctype=NO_OPERATION;
      }

      break;
    }

    case NO_OPERATION:
    {
      // Make thread sleep/Wait
    }

    default:
      throw_error("Wrong Class of Instruction");
  }

  // Copying relevant contents for next pipeline

 // pipeline[2].instr = temp_pipeline[1].instr;
  pipeline[2].rs_val = r1;
  pipeline[2].rt_val = r2;
  pipeline[2].pc = temp_pipeline[1].pc;
  //pipeline[2].no_op = temp_pipeline[1].no_op;
}
sleep(1);
}
}

