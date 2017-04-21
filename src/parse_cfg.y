%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "global_vars.h"
  #include "cache.h"
  extern FILE *parse_cfgin;
  void parse_cfgerror (char const *s);
  int parse_cfglex();
%}

%union
{
  int intval;
  float floatval;
}

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
%token<intval> BOOL
%token<intval> INTEGER
%token<floatval> FLOAT
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
      | PERFECT EQUAL BOOL Icache            {set_cache_param(CACHE_PARAM_IPERF, $3);  /*printf("CACHE_PARAM_IPERF %d\n",$3); */ }
      | SIZE EQUAL INTEGER Icache            {set_cache_param(CACHE_PARAM_ISIZE, 1024*($3));  /*printf("CACHE_PARAM_ISIZE %d\n",$3); */ }
      | ASSOC EQUAL INTEGER Icache           {set_cache_param(CACHE_PARAM_IASSOC, $3);  /*printf("CACHE_PARAM_IASSOC %d\n",$3); */ }
      | REPLACEMENT EQUAL LRU Icache         {set_cache_param(CACHE_PARAM_IREPLACEMENT, 0);  /*printf("CACHE_PARAM_IREPLACEMENT %s\n","lru"); */ }
      | WRITETHROUGH EQUAL INTEGER Icache    {if (($3)) { set_cache_param(CACHE_PARAM_WRITETHROUGH,0); } else { set_cache_param(CACHE_PARAM_WRITEBACK,0); }  /*printf("CACHE_PARAM_IWRITETHROUGH %d\n",$3); */ }
      | BLOCK_SIZE EQUAL INTEGER Icache      {set_cache_param(CACHE_PARAM_IBLOCK_SIZE, $3);  /*printf("CACHE_PARAM_IBLOCK_SIZE %d\n",$3); */ }
Dcache:
      | ICACHE Icache
      | CORE Core
      | DRAM Dram
      | PERFECT EQUAL BOOL Dcache            {set_cache_param(CACHE_PARAM_DPERF, $3);  /*printf("CACHE_PARAM_DPERF %d\n",$3); */ }
      | SIZE EQUAL INTEGER Dcache            {set_cache_param(CACHE_PARAM_DSIZE, 1024*($3));  /*printf("CACHE_PARAM_DSIZE %d\n",$3); */ }
      | ASSOC EQUAL INTEGER Dcache           {set_cache_param(CACHE_PARAM_DASSOC, $3);  /*printf("CACHE_PARAM_DASSOC %d\n",$3); */ }
      | REPLACEMENT EQUAL LRU Dcache         {set_cache_param(CACHE_PARAM_DREPLACEMENT, 0);  /*printf("CACHE_PARAM_DREPLACEMENT %s\n","lru"); */ }
      | WRITETHROUGH EQUAL INTEGER Dcache    {if (($3)) { set_cache_param(CACHE_PARAM_WRITETHROUGH,0); } else { set_cache_param(CACHE_PARAM_WRITEBACK,0); }  /*printf("CACHE_PARAM_DWRITETHROUGH %d\n",$3); */ }
      | BLOCK_SIZE EQUAL INTEGER Dcache      {set_cache_param(CACHE_PARAM_DBLOCK_SIZE, $3);  /*printf("CACHE_PARAM_DBLOCK_SIZE %d\n",$3); */ }
Core:
      | ICACHE Icache
      | DCACHE Dcache
      | DRAM Dram
      | FREQ EQUAL FLOAT Core                {FREQUENCY = $3; /*printf("FREQUENCY %f\n",$3);*/ }
      | FREQ EQUAL INTEGER Core              {FREQUENCY = (float)$3; /*printf("FREQUENCY %d\n",$3);*/ }
Dram:
      | ICACHE Icache
      | DCACHE Dcache
      | CORE Core
      | LAT EQUAL INTEGER Dram               {LATENCY = $3; /*printf("LATENCY %d\n",$3);*/ }

%%

void parse_cfgerror (char const *s)
{
   fprintf (stderr, "Error in format of input file\n");
   exit(-1);
}

// int main(int argc,char* argv[])
// {
//   yyin = fopen("../testcases/test.cfg","r");
//   yyparse();
//   printf("hahah\n");
//   fclose(yyin);
// }