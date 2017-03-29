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

// Control Signals
void draw_mem_write(FILE* ptr, int active);
void draw_mem_read(FILE* ptr, int active);

// Forwading signal
void draw_forward_alu_alu(FILE* ptr, int active);
void draw_forward_mem_alu(FILE* ptr, int active);
void draw_forward_mem_mem(FILE* ptr, int active);

// Stalling signal
void draw_stall(FILE* ptr, int active);

void start_svg(FILE* ptr);
void end_svg(FILE* ptr);

#endif