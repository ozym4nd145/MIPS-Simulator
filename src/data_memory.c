#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void data_memory()
{
	while(1)
	{
		if(read)
		{
			temp_pipeline[2]=pipeline[2];
		}
		else
		{
			pipeline[3]=temp_pipeline[2];
		if(temp_pipeline[2].instr.Itype==NO_OP)
		{
			//sleep;
		}
		if(temp_pipeline[2].instr.Ctype==DT)
		{
			int write_val=temp_pipeline[2].rt_val;
			int offset=temp_pipeline[2].alu_result;

			if((temp_pipeline[3].instr.Itype==LDR_WRD || temp_pipeline[3].instr.Itype==LDR_BYTE) &&  temp_pipeline[3].instr.rt==temp_pipeline[2].instr.rt)
			{
				write_val=temp_pipeline[3].rt_val;
			}
			else if((temp_pipeline[3].instr.Ctype==DP ) &&  temp_pipeline[3].instr.rd==temp_pipeline[2].instr.rt)
			{
				write_val=temp_pipeline[3].rd_val;
			}


			switch(temp_pipeline[2].Itype)
			{
				case LDR_WRD:
				{
					pipeline[3].rt_val=Memory_Block[offset/4];
					break;
				}
				case LDR_BYTE:
				{
					pipeline[3].rt_val=Memory_Block[offset/4];
					break;
				}
				case STR_WRD:
				{
					Memory_Block[offset/4]=write_val;
					break;
				}
				case STR_BYTE:
				{
					Memory_Block[offset/4]=write_val;
					break;
				}
			}

			
		}
	}

	}
}

