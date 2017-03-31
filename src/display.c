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
    if (STOP_THREAD == 1)
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
      char* labels[5] = {"foo", "bar", "bletch", "luffy", "naruto"};

      char* css_name = (char*)malloc(sizeof(char) * (strlen(name) + 20));
      char* js_name = (char*)malloc(sizeof(char) * (strlen(name) + 20));
      strcpy(css_name, name);
      strcpy(js_name, name);
      sprintf(css_name, "%sprocessor.css", name);
      sprintf(js_name, "%sprocessor.js", name);
      FILE* css = fopen(css_name, "w");
      FILE* js = fopen(js_name, "w");
      free(css_name);
      free(js_name);

      start_css(css);

      if (!ACTIVE_STAGE[0])
      {
        fade(css, "fetch");
      }
      if (!ACTIVE_STAGE[1])
      {
        fade(css, "decode");
      }
      if (!ACTIVE_STAGE[2])
      {
        fade(css, "execute");
      }
      if (!ACTIVE_STAGE[3])
      {
        fade(css, "memory");
      }
      if (!ACTIVE_STAGE[4])
      {
        fade(css, "writeback");
      }

      // Activates the corresponding signals
      activate(css, "signal_write");
      // activate(css, "signal_read");
      // activate(css, "signal_forward_from_alu");
      // activate(css, "signal_forward_from_dm");
      // activate(css, "signal_forward_to_alu");
      // activate(css, "signal_forward_to_dm");
      // activate(css, "signal_m2r");
      // activate(css, "signal_flush");
      activate(css, "signal_pcsrc");
      // activate(css, "signal_writeback");
      // activate(css, "signal_stall");

      // Hides corresponding thread from active thread box
      hide(css, "t1");
      hide(css, "t2");
      hide(css, "t3");
      hide(css, "t4");
      // hide(css,"t5");

      // Draws the upper box labels that show instructions in the stage
      draw_js(js, labels);

      fclose(js);
      fclose(css);
      new_fall = 0;
    }
    usleep(DELAY);
  }
  pthread_exit(NULL);
}