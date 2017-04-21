%{
#include "parse_cfg.tab.h"
%}

sp [ \t\n]
eq =
bool (false|true)
natural [0-9]+
float [0-9]*"."{natural}
alphabet [a-zA-Z_]
%option nounput
%option noinput
%s COMMENT

%%

<INITIAL>[ \t\n]+                        {}
<INITIAL>perfect                         { /* printf("PERFECT\n"); */ return PERFECT;}
<INITIAL>cache_size                      { /* printf("SIZE\n"); */ return SIZE;}
<INITIAL>associativity                   { /* printf("ASSOC\n"); */ return ASSOC;}
<INITIAL>replacement_policy              { /* printf("REPLACEMENT\n"); */ return REPLACEMENT;}
<INITIAL>writethrough                    { /* printf("WRITETHROUGH\n"); */ return WRITETHROUGH;}
<INITIAL>block_size                      { /* printf("BLOCK_SIZE\n"); */ return BLOCK_SIZE;}
<INITIAL>frequency                       { /* printf("FREQ\n"); */ return FREQ;}
<INITIAL>latency                         { /* printf("LAT\n"); */ return LAT;}
<INITIAL>lru                             { /* printf("LRU\n"); */ return LRU;}
<INITIAL>{eq}                            { /* printf("EQUAL\n"); */ return EQUAL;}
<INITIAL>{bool}                          { parse_cfglval.intval = (strcmp(parse_cfgtext,"true")==0)?(1):(0); /* printf("BOOL\n"); */ return BOOL;}
<INITIAL>{float}                         { parse_cfglval.floatval = atof(parse_cfgtext); /* printf("FLOAT\n"); */ return FLOAT; }
<INITIAL>{natural}                       { parse_cfglval.intval = atoi(parse_cfgtext); /* printf("INTEGER\n"); */ return INTEGER; }
<INITIAL>#                               { BEGIN COMMENT;}
<INITIAL>\[{alphabet}+\/l1_icache\]      { /* printf("ICACHE\n"); */ return ICACHE;}
<INITIAL>\[{alphabet}+\/l1_dcache\]      { /* printf("DCACHE\n"); */ return DCACHE;}
<INITIAL>\[{alphabet}+\/core\]           { /* printf("CORE\n"); */ return CORE;}
<INITIAL>\[{alphabet}+\/dram\]           { /* printf("DRAM\n"); */ return DRAM;}
<COMMENT>[^\n]*                          {}
<COMMENT>\n                              { BEGIN INITIAL;}
%%
int parse_cfgwrap(void) {
  return 1;
}
