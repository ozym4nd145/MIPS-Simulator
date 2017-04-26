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
  char* css_name = (char*)malloc(sizeof(char) * (strlen(name) + 20));
  char* js_name = (char*)malloc(sizeof(char) * (strlen(name) + 20));
  strcpy(css_name, name);
  strcpy(js_name, name);
  sprintf(css_name, "%sprocessor.css", name);
  sprintf(js_name, "%sprocessor.js", name);
  char* labels[5];
  int i = 0;
  for (i = 0; i < 5; i++)
  {
    labels[i] = (char*)malloc(40 * sizeof(char));
  }
  CLOCK_ZERO_READ[4] = 1;

  while (1)
  {
    int flag = 0;
    if (STOP_THREAD == 1)
    {
      ACTIVE_STAGE[0] = 0;
      ACTIVE_STAGE[1] = 0;
      ACTIVE_STAGE[2] = 0;
      ACTIVE_STAGE[3] = 0;
      ACTIVE_STAGE[4] = 0;
      CONTROL_SIGN.MemWr = 0;
      CONTROL_SIGN.MemRd = 0;
      CONTROL_SIGN.FWD_ALU = 0;
      CONTROL_SIGN.FWD_DM = 0;
      CONTROL_SIGN.TO_ALU = 0;
      CONTROL_SIGN.TO_DM = 0;
      CONTROL_SIGN.M2R = 0;
      CONTROL_SIGN.FLUSH = 0;
      CONTROL_SIGN.PCsrc = 0;
      CONTROL_SIGN.RegW = 0;
      CONTROL_SIGN.STALL_C = 0;
      CURR_INSTR[0].Itype = NO_OP;
      CURR_INSTR[1].Itype = NO_OP;
      CURR_INSTR[2].Itype = NO_OP;
      CURR_INSTR[3].Itype = NO_OP;
      CURR_INSTR[4].Itype = NO_OP;
      flag = 1;
    }
    pthread_mutex_lock(&CLOCK_LOCK);
    if (CLOCK == 0)
    {
      clock_fall = 1;
      CLOCK_ZERO_READ[4] = 1;
    }
    if (CLOCK == 1)
    {
      // indicates that the new instruction is being executed in pipeline
      new_fall = 1;
      clock_fall = 0;
      CLOCK_ZERO_READ[4] = 0;
      SVG_WRITTEN = 0;
    }
    pthread_mutex_unlock(&CLOCK_LOCK);

    if ((clock_fall && new_fall) || (flag && STOP_THREAD))
    {
      FILE* css = fopen(css_name, "w");
      if (css == NULL)
      {
        throw_error("Error in opening output css file");
      }
      FILE* js = fopen(js_name, "w");
      if (js == NULL)
      {
        throw_error("Error in opening output js file");
      }

      for (i = 0; i < 5; i++)
      {
        char* instr = get_instruction_name(CURR_INSTR[i].Itype);
        if (strcmp(instr, "NO_OP") == 0)
        {
          CURR_INSTR[i].index = 0;
        }
        sprintf(labels[i], "%4d: %s", CURR_INSTR[i].index, instr);
      }

      start_css(css);

      if (!ACTIVE_STAGE[0])
      {
        fade(css, "fetch");
        hide(css, "t1");
      }
      if (!ACTIVE_STAGE[1])
      {
        fade(css, "decode");
        hide(css, "t2");
      }
      if (!ACTIVE_STAGE[2])
      {
        fade(css, "execute");
        hide(css, "t3");
      }
      if (!ACTIVE_STAGE[3])
      {
        fade(css, "memory");
        hide(css, "t4");
      }
      if (!ACTIVE_STAGE[4])
      {
        fade(css, "writeback");
        hide(css, "t5");
      }

      // Activates the corresponding signals
      if (CONTROL_SIGN.MemWr) activate(css, "signal_write");
      if (CONTROL_SIGN.MemRd) activate(css, "signal_read");
      if (CONTROL_SIGN.FWD_ALU) activate(css, "signal_forward_from_alu");
      if (CONTROL_SIGN.FWD_DM) activate(css, "signal_forward_from_dm");
      if (CONTROL_SIGN.TO_ALU) activate(css, "signal_forward_to_alu");
      if (CONTROL_SIGN.TO_DM) activate(css, "signal_forward_to_dm");
      if (CONTROL_SIGN.M2R) activate(css, "signal_m2r");
      if (CONTROL_SIGN.FLUSH) activate(css, "signal_flush");
      if (CONTROL_SIGN.PCsrc) activate(css, "signal_pcsrc");
      if (CONTROL_SIGN.RegW) activate(css, "signal_writeback");
      if (CONTROL_SIGN.STALL_C) activate(css, "signal_stall");

      // Draws the upper box labels that show instructions in the stage
      draw_js(js, labels);

      fclose(js);
      fclose(css);
      new_fall = 0;
      SVG_WRITTEN = 1;
    }
    if (STOP_THREAD == 1 && flag == 1)
    {
      free(css_name);
      free(js_name);
      for (i = 0; i < 5; i++) free(labels[i]);
#ifdef DEBUG
      printf("Display Thread Ended\n");
#endif

      break;
    }
    usleep(DELAY);
  }
  pthread_exit(NULL);
}
