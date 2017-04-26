#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "global_vars.h"
void throw_error(char *a)
{
  fprintf(stderr, "%s\n", a);
  exit(-1);
}
void print_instruction(const instruction *instr)
{
  char *name = get_instruction_name(instr->Itype);
  printf("%d %s\n", instr->Itype, name);
}
char *get_instruction_name(const instruction_type instr)
{
  switch (instr)
  {
    case ADD:
      return "ADD";
      break;
    case SUB:
      return "SUB";
      break;
    case AND:
      return "AND";
      break;
    case MULTIPLY:
      return "MUL";
      break;
    case MULTIPLY_ADD:
      return "MADD";
      break;
    case NOR:
      return "NOR";
      break;
    case OR:
      return "OR";
      break;
    case LOGIC_SHIFT_LEFT:
      return "SLL";
      break;
    case LOGIC_SHIFT_LEFT_VARIABLE:
      return "SLLV";
      break;
    case BRANCH_GREATER_OR_EQUAL:
      return "BGEZ";
      break;
    case BRANCH_GREATER:
      return "BGTZ";
      break;
    case BRANCH_LESS_OR_EQUAL:
      return "BLEZ";
      break;
    case BRANCH_LESS:
      return "BLTZ";
      break;
    case BRANCH_EQUAL:
      return "BEQ";
      break;
    case LDR_BYTE:
      return "LB";
      break;
    case LDR_WORD:
      return "LW";
      break;
    case STR_BYTE:
      return "SB";
      break;
    case STR_WORD:
      return "SW";
      break;
    case NO_OP:
      return "NO_OP";
      break;
    case ADDI:
      return "ADDI";
    case ORI:
      return "ORI";
    case LDR_UPPER_IMMEDIATE:
      return "LUI";
    case SLTU:
      return "SLTU";
    case SLTI:
      return "SLTI";
    case JUMP:
      return "JUMP";
    case JUMP_LINK:
      return "JUMP_LINK";
    case JUMP_REGISTER:
      return "JUMP_REGISTER";
    case JUMP_LINK_REGISTER:
      return "JUMP_LINK_REGISTER";
    case MFLO:
      return "MFLO";
    case MOVE:
      return "MOVE";
    case BRANCH_NOT_EQUAL:
      return "BRANCH_NOT_EQUAL";
    case SLL:
      return "SLL";
    case SLT:
      return "SLT";
      break;
  }
  return NULL;
}

char *get_instruction_class(const class_type instr)
{
  switch (instr)
  {
    case DP:
      return "DP";
      break;
    case DT:
      return "DT";
      break;
    case BRANCH:
      return "BRANCH";
      break;
    case NO_OPERATION:
      return "NO_OPERATION";
  }
  return NULL;
}

void int_to_binary(int au)
{
  unsigned a = au;
  printf("Value - %u\n", a);
  char s[35];
  int k = 31;
  s[32] = '\0';
  while (k >= 0)
  {
    if (a % 2 == 0)
      s[k] = '0';
    else
      s[k] = '1';
    a /= 2;
    k--;
  }
  printf("%s\n", s);
}

int lsr(int x, int n) { return (int)((unsigned int)x >> n); }
void regdump()
{
  int i = 0;
  for (i = 0; i < 32; i++)
  {
    printf("$%2d: 0x%08x\n", i, register_file[i]);
  }
  printf("lo: %08x\n", register_file[32]);
  printf("hi: %08x\n", register_file[33]);
  printf("pc: %08x\n", PC);
}

void memdump(int start, int num)
{
  // printf("Inside memdump %08x %d\n", start, num);
  int i = 0;
  for (i = 0; i < num; i++)
  {
    printf("0x%08x: 0x%02x\n", (start + i), get_byte(start + i));
  }
}

int get_byte(int addr)
{
  if (addr < BASE_ADDR || addr > END_ADDR)
  {
    throw_error("Illegal memory access");
  }
  int index = (addr - BASE_ADDR) / 4;
  int offset = (addr - BASE_ADDR) % 4;
  int value = Memory_Block[index];
  int mask = 0xFF;
  int shift = (3 - offset) << 3;
  return lsr((value & (mask << shift)), shift);
}
void instruction_to_file(char *s, buffer instruct)
{
  FILE *write = fopen(s, "a");
  if (write == NULL)
  {
    throw_error("Error in opening output file");
  }
  // fprintf(write, "CLOCK %d\n",CLOCK );
  // fprintf(write, "Number Thread Read %d\n",NUM_THREADS_READ );
  fprintf(write, "--------STEP  %d--------------\n", STEPS);
  fprintf(write, "Register RS %d\n", instruct.instr.rs);
  fprintf(write, "Register RT %d\n", instruct.instr.rt);
  fprintf(write, "Register RD %d\n", instruct.instr.rd);
  fprintf(write, "Immediate %d\n", instruct.instr.immediate);
  fprintf(write, "Register OPCODE %d\n", instruct.instr.opcode);
  fprintf(write, "Register SHT_AMT %d\n", instruct.instr.shf_amt);
  fprintf(write, "Register CTYPE %s\n",
          get_instruction_class(instruct.instr.Ctype));
  fprintf(write, "Register ITYPE %s\n",
          get_instruction_name(instruct.instr.Itype));
  fprintf(write, "Register RS_Val %d\n", instruct.rs_val);
  fprintf(write, "Register RT_Val %d\n", instruct.rt_val);
  fprintf(write, "Register RD_Val %d\n", instruct.rd_val);
  fprintf(write, "Register ALU_Result %d\n", instruct.alu_result);
  fprintf(write, "Register HI_Val %d\n", instruct.HI);
  fprintf(write, "Register LO_Val %d\n", instruct.LO);
  fprintf(write, "Register PC_Val %08x\n", instruct.pc);
  fprintf(write, "---------------------------\n");
  fprintf(write, "\n\n");
  fclose(write);
}

void print_registers(char *s)
{
  FILE *write = fopen(s, "a");
  if (write == NULL)
  {
    throw_error("Error in opening output file");
  }
  int i;
  fprintf(write, "--------STEP  %d--------------\n", STEPS);
  for (i = 0; i < 32; i++)
    fprintf(write, "Register %d :%d\n", i, register_file[i]);
  fprintf(write, "Register LO: %d\n", register_file[32]);
  fprintf(write, "Register HI: %d\n", register_file[33]);
  fprintf(write, "---------------------------\n");

  fprintf(write, "\n\n");
  fclose(write);
}

void make_html(char *svg_name)
{
  char *html_upper =
      "<html>\n<head>\n    <title>MIPS Pipelined "
      "Simulator</title>\n</head>\n<body>\n    <img id=\"processor\" "
      "src=\"";
  char *html_lower =
      "\" />\n    <script "
      "src=\"./refresh.js\"> </script>\n</body>\n</html>";
  FILE *html = fopen("image.html", "w");
  fprintf(html, "%s %s %s", html_upper, svg_name, html_lower);
  fclose(html);
}

void print_result(char *s)
{
  // fprintf(stdout, "Reached\n" );
  // FILE *write=stdout;
  FILE *write = fopen(s, "w");
  if (write == NULL)
  {
    throw_error("Error in opening result file");
  }
  // fprintf(write, "Reached\n" );
  double cycles_waste_on_miss = ceil(LATENCY * FREQUENCY);
  double cycles =
      (STEPS - 1) +
      (cache_stat_inst.num_mem_access + cache_stat_data.num_mem_access) *
          cycles_waste_on_miss;
  double Ins_C = INSTRUCTION_COUNT;
  double IPC = Ins_C / cycles;
  double time_exe = cycles * (1.0 / FREQUENCY);
  double idle_time = ((double)(-Ins_C + cycles)) * 0.5;
  double idle_time_percent = (idle_time / time_exe) * 100;
  // printf("BRANCH_CYCLE_WASTE%d\n", BRANCH_CYCLE_WASTE);
  // printf("STALL_WASTE%d\n", STALL_COUNT);
  // fprintf(write, "Reached\n" );
  // fprintf(write, "STALL_COUNT %d\n",STALL_COUNT );
  fprintf(write, "Instructions,%d\n", INSTRUCTION_COUNT);
  fprintf(write, "Cycles,%d\n", (int)cycles);
  fprintf(write, "IPC,%.4lf\n", IPC);
  fprintf(write, "Time (ns),%.4lf\n", time_exe);
  fprintf(write, "Idle Time (ns),%.4lf\n", idle_time);
  fprintf(write, "Idle Time (%%),%.4lf%%\n", idle_time_percent);
  fprintf(write, "Cache Summary\n");
  fprintf(write, "Cache L1-I\n");
  fprintf(write, "num cache accesses,%d\n", cache_stat_inst.accesses);
  fprintf(write, "num cache misses,%d\n", cache_stat_inst.misses);
  fprintf(write, "miss rate,%.4f%%\n",
          ((cache_stat_inst.misses * 1.0) / cache_stat_inst.accesses) * 100.0);
  fprintf(write, "Cache L1-D\n");
  fprintf(write, "num cache accesses,%d\n", cache_stat_data.accesses);
  fprintf(write, "num cache misses,%d\n", cache_stat_data.misses);
  fprintf(write, "miss rate,%.4f%%\n",
          ((cache_stat_data.misses * 1.0) / cache_stat_data.accesses) * 100.0);
  fprintf(write, "DRAM summary\n");
  fprintf(write, "num dram accesses,%d\n",
          (cache_stat_inst.accesses + cache_stat_data.accesses));
  fprintf(write, "average dram access latency (ns),%.4f\n",
          (1.0 * LATENCY * (cache_stat_data.misses + cache_stat_inst.misses)) /
              (cache_stat_inst.accesses + cache_stat_data.accesses));
  fclose(write);
}

// Mode =1 => read from memory| Mode !=1 => write to memory
int program_memory_interface(int val, int address, int mode)
{
  if (address < BASE_ADDR || address > END_ADDR)
  {
    throw_error("Illegal Data Memory Access");
  }

  if (mode == 1)
  {
    perform_access(address, TRACE_DATA_LOAD);
#ifdef DEBUG
    fprintf(trace_file, "0 %x\n", address);
#endif
    return Memory_Block[(address - BASE_ADDR) / 4];
  }
  else
  {
    perform_access(address, TRACE_DATA_STORE);
#ifdef DEBUG
    fprintf(trace_file, "1 %x\n", address);
#endif

    Memory_Block[(address - BASE_ADDR) / 4] = val;
    return 1;
  }
}

instruction program_instruction_interface(int address)
{
  if (address < BASE_PC_ADDR)
  {
    throw_error("Illegal Memory Access");
  }
  perform_access(address, TRACE_INST_LOAD);
#ifdef DEBUG
  fprintf(trace_file, "2 %x\n", address);
#endif
  return program[(address - BASE_PC_ADDR) / 4];
}
