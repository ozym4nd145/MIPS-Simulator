#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"
#include "global_vars.h"
#include "main_functions.h"
#include "parse_cfg.tab.h"
#include "svg.h"
#include "utils.h"

int main(int argc, char* argv[])
{
  FILE* code;
  FILE* svg;
  // Input error handling
  if (argc == 5)
  {
    code = fopen(argv[1], "r");
  }
  else
  {
    fprintf(stderr,
            "Usage: %s <input_HEX_file> <input_CFG_file> <output_SVG_file> "
            "<output_results_file>\n",
            argv[0]);
    throw_error("");
  }

  if (code == NULL)
  {
    throw_error("Error in opening instruction file");
  }
  // Drawing the svg
  svg = fopen(argv[3], "w");
  if (svg == NULL)
  {
    throw_error("Error in opening svg file");
  }

  draw_svg(svg);
  fclose(svg);

  init_cache();
  parse_cfgin = fopen(argv[2], "r");
  if (parse_cfgin == NULL)
  {
    throw_error("Error in opening config file");
  }
  parse_cfgparse();
  fclose(parse_cfgin);

  // Calculating base path of svg
  char* base_name = strdup(argv[3]);
  int name_start_index = strlen(argv[3]) - 1;
  while (name_start_index >= 0)
  {
    if (argv[2][name_start_index] == '/')
    {
      break;
    }
    name_start_index--;
  }
  base_name[name_start_index + 1] = '\0';

  char* a = malloc(sizeof(char) * 10);
  int i = 0;

  // reading and parsing instructions until file end
  while (fscanf(code, "%s", a) != EOF)
  {
    int num = (int)strtol(a, NULL, 16);
    program[i] = instruction_parse(num);
    program[i].index = i + 1;
    i++;
  }
  free(a);

  // // Making html for displaying svg
  // make_html(argv[2]);

  // Initializing MAX_PC
  MAX_PC = (4 * (i - 1)) + BASE_PC_ADDR;

  for (; i >= 0; i--) BreakPoint[i] = 0;

  // Initializing memory and pipeline buffer
  for (i = 0; i <= (NUM_THREADS - 1); i++)
  {
    pipeline[i].instr.Itype = NO_OP;
    pipeline[i].instr.Ctype = NO_OPERATION;
  }

  for (i = 0; i < MEMORY_SIZE; i++)
  {
    // Memory_Block[i] = 1;  // Just for checking
    Memory_Block[i] = 0;
  }

  // Initialize register file
  for (i = 0; i < 32; i++)
  {
    // register_file[i] = i + 1;  // Just for checking
    register_file[i] = 0;
  }

  // Initializing stack pointer
  register_file[29] = END_ADDR - 4;

  for (i = 0; i < NUM_THREADS; i++)
  {
    ACTIVE_STAGE[i] = 0;
    CURR_INSTR[i].Itype = NO_OP;
    CURR_INSTR[i].Ctype = NO_OPERATION;
  }

  control_signal.stall = 0;
  control_signal.branched = 0;

  // Initializing mutex locks
  pthread_mutex_init(&CLOCK_LOCK, NULL);
  pthread_mutex_init(&READ_LOCK, NULL);
  pthread_mutex_init(&WRITE_LOCK, NULL);

  // Creating threads
  pthread_create(&threads[0], NULL, instruction_fetch, (void*)NULL);
  pthread_create(&threads[1], NULL, register_read, (void*)NULL);
  pthread_create(&threads[2], NULL, alu_op, (void*)NULL);
  pthread_create(&threads[3], NULL, memory_op, (void*)NULL);
  pthread_create(&threads[4], NULL, register_write, (void*)NULL);
  pthread_create(&threads[5], NULL, print_svg, (void*)(base_name));

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  pthread_join(threads[2], NULL);
  pthread_join(threads[3], NULL);
  pthread_join(threads[4], NULL);
  pthread_join(threads[5], NULL);

#ifdef DEBUG
  printf("Control  Reached here\n");
#endif
  print_result(argv[4]);

  free(base_name);
  fclose(code);

  return 0;
}
