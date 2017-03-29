#ifndef UTILS_H
#define UTILS_H

#include "global_vars.h"

void throw_error(char *a);
void print_instruction(const instruction *);
char *get_instruction_name(const instruction_type instr);
void int_to_binary(int a);
int lsr(int x, int n);
void instruction_to_file(char *s, buffer instruct);
char *get_instruction_class(const class_type instr);
void print_registers(char *s);
void memdump(int start, int num);
int get_byte(int addr);
void regdump();
void make_html(char *svg_name);
#endif