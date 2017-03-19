
void data_memory()
{
	while(1)
	{
		if(pipeline[2].instr.Itype==NO_OP)
		{
			//sleep;
			pipeline[3].instr.Itype=NO_OP;
		}
		if(pipeline[2].instr.Ctype==DT)
		{
			int write_val=pipeline[2].rt_val;
			int offset=pipeline[2].alu_result;

			if((pipeline[3].instr.Itype==LDR_WRD || pipeline[3].instr.Itype==LDR_BYTE) &&  pipeline[3].instr.rt==pipeline[2].instr.rt)
			{
				write_val=pipeline[3].rt_val;
			}
			else if((pipeline[3].instr.Ctype==DP ) &&  pipeline[3].instr.rd==pipeline[2].instr.rt)
			{
				write_val=pipeline[3].rd_val;
			}


			switch(pipeline[2].Itype)
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
		pipeline[3].instr=pipeline[2].instr;
		pipeline[3].alu_result=pipeline[2].alu_result;
		pipeline[3].rs_val=write_val;
		pipeline[3].pc = pipeline[2].pc;

	}
}

