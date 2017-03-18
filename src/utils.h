#ifndef UTILS_H
#define UTILS_H

#include "global_vars.h"

void throw_error(char *a);
void print_instruction(const instruction *);
char *get_instruction_name(const instruction_type instr);

#endif