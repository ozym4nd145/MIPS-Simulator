#ifndef SVG_H
#define SVG_H

#include <stdio.h>

// Main components
void draw_fetch(FILE* ptr, int active);
void draw_decode(FILE* ptr, int active);
void draw_alu(FILE* ptr, int active);
void draw_memory(FILE* ptr, int active);
void draw_write(FILE* ptr, int active);
void draw_control(FILE* ptr);
void draw_mem_read(FILE* ptr, int active);
void draw_mem_write(FILE* ptr, int active);
void draw_forward_toalu(FILE* ptr, int active);
void draw_forward_fromalu(FILE* ptr, int active);
void draw_forward_tomemory(FILE* ptr, int active);
void draw_forward_frommemory(FILE* ptr, int active);
void draw_pcsrc(FILE* ptr, int active);
void draw_memtoreg(FILE* ptr, int active);
void draw_writeback(FILE* ptr, int active);
void draw_stall(FILE* ptr, int active);

void start_svg(FILE* ptr);
void end_svg(FILE* ptr);

#endif