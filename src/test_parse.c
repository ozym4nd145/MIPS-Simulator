#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "parse_cfg.tab.h"

extern FILE* parse_cfgin;
extern int parse_cfgparse(void);

int main()
{
  FILE* fl = fopen("../testcases/test.cfg", "r");
  parse_cfgin = fl;
  parse_cfgparse();
  fclose(fl);
  return 0;
}