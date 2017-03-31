#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

instruction instruction_parse(int a);
void* instruction_fetch(void* data);
void* register_read(void* data);
void* register_write(void* data);
void* alu_op(void* data);
void* memory_op(void* data);
void* print_svg(void* data);

#endif
