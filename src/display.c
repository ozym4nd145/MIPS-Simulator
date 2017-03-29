#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "global_vars.h"
#include "main_functions.h"
#include "svg.h"
#include "utils.h"

void* print_svg(void* data)
{
  char* name = (char*)data;
  int clock_fall = 0;
  int new_fall = 1;

  while (1)
  {

    if(STOP_THREAD==1)
    {
      printf("Display thread Stopped\n");
      break;
    }
    pthread_mutex_lock(&CLOCK_LOCK);
    if (CLOCK == 0)
    {
      clock_fall = 1;
    }
    if (CLOCK == 1)
    {
      // indicates that the new instruction is being executed in pipeline
      new_fall = 1;
      clock_fall = 0;
    }
    pthread_mutex_unlock(&CLOCK_LOCK);

    if (clock_fall && new_fall)
    {
      FILE* svg = fopen(name, "w");
      start_svg(svg);
      draw_fetch(svg, ACTIVE_STAGE[0]);
        draw_decode(svg, ACTIVE_STAGE[1]);
        draw_alu(svg, ACTIVE_STAGE[2]);
        draw_memory(svg, ACTIVE_STAGE[3]);
        draw_write(svg, ACTIVE_STAGE[4]);
      draw_control(svg);
      end_svg(svg);
      fclose(svg);
      new_fall = 0;
    }
    usleep(DELAY);
  }
  pthread_exit(NULL);
}