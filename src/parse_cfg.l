%{
#include <stdio.h>
#include "y.tab.h"
%}

sp [ \t\n]
eq =
bool (false|true)
natural [0-9]+
alphabet [a-zA-Z_]

%s COMMENT

%%

<INITIAL>[ \t\n]+                        {}
<INITIAL>perfect                         { return PERFECT;}
<INITIAL>cache_size                      { return SIZE;}
<INITIAL>associativity                   { return ASSOC;}
<INITIAL>replacement_policy              { return REPLACEMENT;}
<INITIAL>writethrough                    { return WRITETHROUGH;}
<INITIAL>block_size                      { return BLOCK_SIZE;}
<INITIAL>frequency                       { return FREQ;}
<INITIAL>latency                         { return LAT;}
<INITIAL>lru                             { return LRU;}
<INITIAL>{eq}                            { return EQUAL;}
<INITIAL>{bool}                          { yylval = (strcmp(yytext,"true")==0)?(1):(0); return BOOL;}
<INITIAL>{natural}                       { yylval = atoi(yytext); return INTEGER; }
<INITIAL>#                               { BEGIN COMMENT;}
<INITIAL>\[{alphabet}+\/l1_icache\]      { return ICACHE;}
<INITIAL>\[{alphabet}+\/l1_dcache\]      { return DCACHE;}
<INITIAL>\[{alphabet}+\/core\]           { return CORE;}
<INITIAL>\[{alphabet}+\/dram\]           { return DRAM;}
<COMMENT>[^\n]*                          {}
<COMMENT>\n                              { BEGIN INITIAL;}
%%
int yywrap(void) {
  return 1;
}
