%{
  #include <stdio.h>
  #include "global_vars.h"
  #include "cache.h"
  extern FILE *yyin;
  void yyerror (char const *s);
  int yylex();
%}

%token PERFECT
%token SIZE
%token ASSOC
%token REPLACEMENT
%token WRITETHROUGH
%token BLOCK_SIZE
%token FREQ
%token LAT
%token LRU
%token EQUAL
%token BOOL
%token INTEGER
%token ICACHE
%token DCACHE
%token CORE
%token DRAM
%token End_Of_File

%start Input

%%
Input:  ICACHE Icache  
      | DCACHE Dcache 
      | CORE Core     
      | DRAM Dram     

Icache:
      | DCACHE Dcache
      | CORE Core
      | DRAM Dram
      | PERFECT EQUAL BOOL Icache            {/* set_cache_param(CACHE_PARAM_IPERF, $3); */ printf("CACHE_PARAM_IPERF %d\n",$3); }
      | SIZE EQUAL INTEGER Icache            {/* set_cache_param(CACHE_PARAM_ISIZE, $3); */ printf("CACHE_PARAM_ISIZE %d\n",$3); }
      | ASSOC EQUAL INTEGER Icache           {/* set_cache_param(CACHE_PARAM_IASSOC, $3); */ printf("CACHE_PARAM_IASSOC %d\n",$3); }
      | REPLACEMENT EQUAL LRU Icache         {/* set_cache_param(CACHE_PARAM_IREPLACEMENT, LRU); */ printf("CACHE_PARAM_IREPLACEMENT %s\n","lru"); }
      | WRITETHROUGH EQUAL INTEGER Icache    {/* set_cache_param(CACHE_PARAM_IWRITETHROUGH, $3); */ printf("CACHE_PARAM_IWRITETHROUGH %d\n",$3); }
      | BLOCK_SIZE EQUAL INTEGER Icache      {/* set_cache_param(CACHE_PARAM_IBLOCK_SIZE, $3); */ printf("CACHE_PARAM_IBLOCK_SIZE %d\n",$3); }
Dcache:
      | ICACHE Icache
      | CORE Core
      | DRAM Dram
      | PERFECT EQUAL BOOL Dcache            {/* set_cache_param(CACHE_PARAM_DPERF, $3); */ printf("CACHE_PARAM_DPERF %d\n",$3); }
      | SIZE EQUAL INTEGER Dcache            {/* set_cache_param(CACHE_PARAM_DSIZE, $3); */ printf("CACHE_PARAM_DSIZE %d\n",$3); }
      | ASSOC EQUAL INTEGER Dcache           {/* set_cache_param(CACHE_PARAM_DASSOC, $3); */ printf("CACHE_PARAM_DASSOC %d\n",$3); }
      | REPLACEMENT EQUAL LRU Dcache         {/* set_cache_param(CACHE_PARAM_DREPLACEMENT, LRU_POLICY); */ printf("CACHE_PARAM_DREPLACEMENT %s\n","lru"); }
      | WRITETHROUGH EQUAL INTEGER Dcache    {/* set_cache_param(CACHE_PARAM_DWRITETHROUGH, $3); */ printf("CACHE_PARAM_DWRITETHROUGH %d\n",$3); }
      | BLOCK_SIZE EQUAL INTEGER Dcache      {/* set_cache_param(CACHE_PARAM_DBLOCK_SIZE, $3); */ printf("CACHE_PARAM_DBLOCK_SIZE %d\n",$3); }
Core:
      | ICACHE Icache
      | DCACHE Dcache
      | DRAM Dram
      | FREQ EQUAL INTEGER Core            {/*FREQUENCY = $3;*/printf("FREQUENCY %d\n",$3);}
Dram:
      | ICACHE Icache
      | DCACHE Dcache
      | CORE Core
      | LAT EQUAL INTEGER Dram         {/*LATENCY = $3;*/printf("LATENCY %d\n",$3);}

%%

void yyerror (char const *s) {
   fprintf (stderr, "Error in format of input file\n");
   exit(-1);
}

int main(int argc,char* argv[])
{
  yyin = fopen("../testcases/test.cfg","r");
  yyparse();
  printf("hahah\n");
  fclose(yyin);
}