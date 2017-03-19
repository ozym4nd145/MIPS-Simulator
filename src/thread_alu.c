
void alu_op()
{

	switch(pipeline[1].instr.Ctype)
	{

			int r1=pipeline[1].rs_val;
			int r2=pipeline[1].rt_val;

			if(pipeline[1].instr.rs==pipeline[2].instr.rd && pipeline[2].instr.Ctype==DP)
			{
				r1=pipeline[2].alu_result;
			}
			else if(pipeline[1].instr.rs==pipeline[3].instr.rt && pipeline[3].instr.Itype==LDR_WRD)
			{
				r1=pipeline[3].rt_val;
			}

			if(pipeline[1].instr.rt==pipeline[2].instr.rd && pipeline[2].instr.Ctype==DP)
			{
				r2=pipeline[2].alu_result;
			}
			else if(pipeline[1].instr.rt==pipeline[3].instr.rt && pipeline[3].instr.Itype==LDR_WRD)
			{
				r2=pipeline[3].rt_val;
			}


		case DP:
		{
			switch(pipeline[1].instr.Itype)
			{
				case ADD:
				pipeline[2].alu_result=r1+r2;
				break;

				case SUB:
				pipeline[2].alu_result=r1-r2;
				break;

				case NOR:
				pipeline[2].alu_result=~(r1|r2);
				break;

				case OR:
				pipeline[2].alu_result=(r1|r2);
				break;

				case LOGICAL_SHIFT_LEFT:
				pipeline[2].alu_result=(r2<<(pipeline[1].instr.shft_amt));
				break;

				case AND:
				pipeline[2].alu_result=r1&r2;
				break;

				case LOGIC_SHIFT_LEFT_VARIABLE:
				pipeline[2].alu_result=r2<<(r1&(0x0000001F));
				break;

				case MULTIPLY:
				long long int temp=((long long int)r1)*((long long int)r2);
				pipeline[2].HI=(int)((unsigned temp)>>32);
				pipeline[2].LO=(int)temp;
				break;

				case MULTIPLY_ADD:
				long long int temp=((long long int)r1)*((long long int)r2);
				pipeline[2].HI+=(int)((unsigned temp)>>32);
				pipeline[2].LO+=(int)temp;
				break;

				default:
				throw_error("Unrecognized Instruction");
		}
		break;
	}

	case DT:
	{
		switch(pipeline[1].instr.Itype)
		{
			case LDR_WRD:
			case LDR_BYTE:
			case STR_WRD:
			case STR_BYTE:
			pipeline[2].alu_result=pipeline[1].instr.immediate+pipeline[1].rs_val;
			break;
			default:
			throw_error("Unrecognized Instruction");
		}
		break;
	}
	//Branch Class Cases Handling
	case BRANCH:
	{
		switch(pipeline[1].instr.Itype)
		{
			case BRANCH_EQUAL:
			if(pipeline[1].rs_val==pipeline[1].rt_val)
			{
				PC+=pipeline[1].pc+(pipeline[1].instr.immediate<<4);
			}
			break;

			case BRANCH_GREATER_OR_EQUAL:
			if(pipeline[1].rs_val>=0)
			{
				PC+=pipeline[1].pc+(pipeline[1].instr.immediate<<4);
			}
			break;

			case BRANCH_LESS_OR_EQUAL:
			if(pipeline[1].rs_val<=0)
			{
				PC+=pipeline[1].pc+(pipeline[1].instr.immediate<<4);
			}
			break;

			case BRANCH_GREATER:
			if(pipeline[1].rs_val>0)
			{
				PC+=pipeline[1].pc+(pipeline[1].instr.immediate<<4);
			}
			break;

			case BRANCH_LESS:
			if(pipeline[1].rs_val<0)
			{
				PC+=pipeline[1].pc+(pipeline[1].instr.immediate<<4);
			}
			break;

			default:
			throw_error("Wrong Instruction");
		}
		break;
	}

	case NO_OP:
	{
		//Make thread sleep/Wait 
	}

	default:
	throw_error("Wrong Class of Instruction");

	}

	//Copying relevant contents for next pipeline

	pipeline[2].instr=pipeline[1].instr;
	pipeline[2].rs_val=pipeline[1].rs_val;
	pipeline[2].rt_val=pipeline[1].rt_val;
	pipeline[2].pc=pipeline[1].pc;
	pipeline[2].no_op=pipeline[1].no_op;

}
