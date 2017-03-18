
void alu_op()
{
	switch(pipeline[1].instr.Ctype)
	{
		case DP:
		{
			int r1=pipeline[1].rs_val;
			int r2=pipeline[1].rt_val;

			if(pipeline[1].instr.rs==pipeline[2].instr.rd && pipeline[2].instr.Ctype==DP)
			{
				r1=pipeline[2].alu_result;
			}
			else if(pipeline[1].instr.rs==pipeline[2].instr.rd && pipeline[2].instr.Itype==LDR_WRD)

		}
	}
}