#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
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
      return "MULTIPLY";
      break;
    case MULTIPLY_ADD:
      return "MULTIPLY_ADD";
      break;
    case NOR:
      return "NOR";
      break;
    case OR:
      return "OR";
      break;
    case LOGIC_SHIFT_LEFT:
      return "LOGIC_SHIFT_LEFT";
      break;
    case LOGIC_SHIFT_LEFT_VARIABLE:
      return "LOGIC_SHIFT_LEFT_VARIABLE";
      break;
    case SUBTRACT:
      return "SUBTRACT";
      break;
    case BRANCH_GREATER_OR_EQUAL:
      return "BRANCH_GREATER_OR_EQUAL";
      break;
    case BRANCH_GREATER:
      return "BRANCH_GREATER";
      break;
    case BRANCH_LESS_OR_EQUAL:
      return "BRANCH_LESS_OR_EQUAL";
      break;
    case BRANCH_LESS:
      return "BRANCH_LESS";
      break;
    case BRANCH_EQUAL:
      return "BRANCH_EQUAL";
      break;
    case LDR_BYTE:
      return "LDR_BYTE";
      break;
    case LDR_WORD:
      return "LDR_WORD";
      break;
    case STR_BYTE:
      return "STR_BYTE";
      break;
    case STR_WORD:
      return "STR_WORD";
      break;
  }
  return NULL;
}

void int_to_binary(int au)
{
  unsigned a = au;
  printf("Value - %u\n",a);
  char s[35];int k=31;
  s[32]='\0';
  while(k>=0)
  {
    if(a%2==0)
      s[k]='0';
    else s[k]='1';
    a/=2;
    k--;
  }
  printf("%s\n",s);
}

int lsr(int x,int n)
{
  return (int)((unsigned int)x >> n);
}