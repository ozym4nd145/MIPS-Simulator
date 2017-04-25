#ifndef CACHE_H
#define CACHE_H

/*
 * cache.h
 */

#define TRUE 1
#define FALSE 0

/* default cache parameters--can be changed */
#define WORD_SIZE 4
#define WORD_SIZE_OFFSET 2
#define DEFAULT_CACHE_SIZE (8 * 1024)
#define DEFAULT_CACHE_BLOCK_SIZE 16
#define DEFAULT_CACHE_ASSOC 1
#define DEFAULT_CACHE_WRITEBACK TRUE
#define DEFAULT_CACHE_WRITEALLOC TRUE
#define DEFAULT_CACHE_PERFECT FALSE
#define DEFAULT_CACHE_REPLACEMENT LRU_POLICY
#define TRACE_DATA_LOAD 0
#define TRACE_DATA_STORE 1
#define TRACE_INST_LOAD 2

/* constants for settting cache parameters */
typedef enum {
  CACHE_PARAM_BLOCK_SIZE,
  CACHE_PARAM_IBLOCK_SIZE,
  CACHE_PARAM_DBLOCK_SIZE,
  CACHE_PARAM_USIZE,
  CACHE_PARAM_ISIZE,
  CACHE_PARAM_IPERF,
  CACHE_PARAM_DSIZE,
  CACHE_PARAM_DPERF,
  CACHE_PARAM_ASSOC,
  CACHE_PARAM_IASSOC,
  CACHE_PARAM_DASSOC,
  CACHE_PARAM_WRITEBACK,
  CACHE_PARAM_WRITETHROUGH,
  CACHE_PARAM_WRITEALLOC,
  CACHE_PARAM_NOWRITEALLOC,
  CACHE_PARAM_IREPLACEMENT,
  CACHE_PARAM_DREPLACEMENT,
} cache_param;

typedef enum { LRU_POLICY } cache_replacement;

/* structure definitions */
typedef struct cache_line_
{
  unsigned tag;
  int dirty;

  struct cache_line_ *LRU_next;
  struct cache_line_ *LRU_prev;
} cache_line, *Pcache_line;

typedef struct cache_set_
{
  Pcache_line head;
  Pcache_line tail;
  int set_contents_count;
  int max_set_contents_count;

} cache_set, *Pcache_set;

typedef struct cache_
{
  int size;              /* cache size in words*/
  int associativity;     /* cache associativity */
  int n_sets;            /* number of cache sets */
  unsigned index_mask;   /* mask to find cache index */
  unsigned tag_mask;     /* mask to find cache index */
  int index_mask_offset; /* number of zero bits in mask */
  int tag_mask_offset;
  Pcache_set *set;
  // Pcache_line *LRU_head; /* head of LRU list for each set */
  // Pcache_line *LRU_tail; /* tail of LRU list for each set */
  // int *set_contents;     /* number of valid entries in set */
  int contents; /* number of valid entries in cache */
} cache, *Pcache;

typedef struct cache_stat_
{
  int accesses;       /* number of memory references */
  int misses;         /* number of cache misses */
  int replacements;   /* number of misses that cause replacments */
  int demand_fetches; /* number of fetches */
  int copies_back;    /* number of write backs */
  int num_mem_access; /* number of memory accesses (read as well as write) */
} cache_stat, *Pcache_stat;

/* function prototypes */
void set_cache_param();
void init_cache();
void perform_access();
void flush();
void delete (Pcache_line *head, Pcache_line *tail, Pcache_line item);
void insert(Pcache_line *head, Pcache_line *tail, Pcache_line item);
void dump_settings();
void print_stats();
void print_eval_stats();
void init_cache_params(Pcache c, int cache_size, int block_size, int wpb,
                       int assoc);
extern cache_stat cache_stat_inst;
extern cache_stat cache_stat_data;
/* macros */
#define LOG2(x) ((int)rint((log((double)(x))) / (log(2.0))))

#endif