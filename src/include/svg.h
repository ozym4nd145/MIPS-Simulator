#ifndef SVG_H
#define SVG_H

#include <stdio.h>

void draw_svg(FILE* ptr);
void start_css(FILE* ptr);
void draw_js(FILE* ptr, char* text[]);
void fade(FILE* ptr, char* label);
void activate(FILE* ptr, char* label);
void hide(FILE* ptr, char* label);

#endif