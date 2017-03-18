#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv)
{
  FILE* code;
  FILE* svg;
  if (argc == 3)
  {
    code = fopen(argv[1], "r");
    svg = fopen(argv[2], "w");
  }
  else
  {
    fprintf(stderr, "Usage: %s <instruction_file> <svg_output>\n", argv[0]);
  }
}