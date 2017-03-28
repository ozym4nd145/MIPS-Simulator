#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"
// typedef enum { Monday, Tuesday Thursday} days;

int input(FILE *fl)
{
  char *a = malloc(sizeof(char) * 10);
  fscanf(fl, "%s", a);
  int num = (int)strtol(a, NULL, 16);
  free(a);
  return num;
}

instruction instruction_parse(int a)
{
  // printf("Instruction %08x\n", a);
  instruction *information = (instruction *)malloc(sizeof(instruction));

  int temp = 63;

  information->opcode = lsr((a & (temp << 26)), 26);

  // printf("%d\n",information->opcode);
  temp = (temp >> 1);
  information->rs = lsr((a & (temp << 21)), 21);
  information->rt = lsr((a & (temp << 16)), 16);
  information->rd = lsr((a & (temp << 11)), 11);
  information->shf_amt = lsr((a & (temp << 6)), 6);
  information->function = a & ((temp << 1) | 1);
  information->immediate = a & (0xFFFF);
  information->immediate = (information->immediate << 16) >> 16;
  //printf("%08x\n",information->immediate);
  
  information->target_address = a&(0x3FFFFFF);
  if (information->opcode == 0 && information->function == 32 &&
      information->shf_amt == 0)
  {
    information->Ctype = DP;
    information->Itype = ADD;
  }
  else if (information->opcode == 8)
  {
    information->Ctype = DP;
    information->Itype = ADDI;
  }
  else if (information->opcode == 0 && information->shf_amt == 0 &&
           information->function == 36)
  {
    information->Ctype = DP;
    information->Itype = AND;
  }
  else if (information->opcode == 0 && information->immediate == 24)
  {
    information->Ctype = DP;
    information->Itype = MULTIPLY;
  }
  else if (information->opcode == 28 && information->immediate == 0)
  {
    information->Ctype = DP;
    information->Itype = MULTIPLY_ADD;
  }
  else if (information->opcode == 0 && information->shf_amt == 0 &&
           information->function == 39)
  {
    information->Ctype = DP;
    information->Itype = NOR;
  }
  else if (information->opcode == 0 && information->shf_amt == 0 &&
           information->function == 37)
  {
    information->Ctype = DP;
    information->Itype = OR;
  }
  else if (information->opcode == 13)
  {
    information->Ctype = DP;
    information->Itype = ORI;
  }
  else if (information->opcode == 0 && information->function == 0)
  {
    information->Ctype = DP;
    information->Itype = LOGIC_SHIFT_LEFT;
  }
  else if (information->opcode == 0 && information->function == 4)
  {
    information->Ctype = DP;
    information->Itype = LOGIC_SHIFT_LEFT_VARIABLE;
  }
  else if (information->opcode == 0 && information->function == 34 &&
           information->shf_amt == 0)
  {
    information->Ctype = DP;
    information->Itype = SUB;
  }
  else if (information->opcode == 4)
  {
    information->Ctype = BRANCH;
    information->Itype = BRANCH_EQUAL;
  }
  else if (information->opcode == 1 && information->rt == 1)
  {
    information->Ctype = BRANCH;
    information->Itype = BRANCH_GREATER_OR_EQUAL;
  }
  else if (information->opcode == 7 && information->rt == 0)
  {
    information->Ctype = DT;
    information->Itype = BRANCH_GREATER;
  }
  else if (information->opcode == 6 && information->rt == 0)
  {
    information->Ctype = DT;
    information->Itype = BRANCH_LESS_OR_EQUAL;
  }
  else if (information->opcode == 1 && information->rt == 0)
  {
    information->Ctype = DT;
    information->Itype = BRANCH_LESS;
  }
  else if (information->opcode == 32)
  {
    information->Ctype = DT;
    information->Itype = LDR_BYTE;
  }
  else if (information->opcode == 35)
  {
    information->Ctype = DT;
    information->Itype = LDR_WORD;
  }
  else if (information->opcode == 40)
  {
    information->Ctype = DT;
    information->Itype = STR_BYTE;
  }
  else if (information->opcode == 43)
  {
    information->Ctype = DT;
    information->Itype = STR_WORD;
  }
  else if (information->opcode == 15)
  {
    information->Ctype = DT;
    information->Itype = LDR_UPPER_IMMEDIATE;
  }
  else if (information->opcode == 0 && information->shf_amt == 0 &&
           information->function == 43)
  {
    information->Ctype = DP;
    information->Itype = SLTU;
  }
  else if (information->opcode == 10)
  {
    information->Ctype = DP;
    information->Itype = SLTI;
  }
  else
  {
    throw_error("Wrong Instruction Set.");
  }

  return *information;
}