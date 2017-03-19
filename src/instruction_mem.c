#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global_vars.h"
#include "main_functions.h"
#include "utils.h"

void* instruction_fetch(void* data)
{
  printf("Inside Instruction fetch\n");
  char input[100];
  while (1)
  {
    scanf("%s", input);
    if (strcmp(input, "step") == 0)
    {
      STEPS++;
      if (control_signal.stall == 0)
      {
        pipeline[0].instr = program[PC / 4];
        pipeline[0].pc = PC;
        PC = PC + 4;
        printf("PC - %d\n", PC);
      }
      else
      {
        control_signal.stall = 0;
      }
      sleep(1);
    }
  }
}