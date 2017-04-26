/*
 * cache.c
 */

#include "cache.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru.h"
#include "trace.h"

/* cache configuration parameters */
static int cache_split = 0;
static int cache_usize = DEFAULT_CACHE_SIZE;
static int cache_isize = DEFAULT_CACHE_SIZE;
static int cache_dsize = DEFAULT_CACHE_SIZE;
static int cache_iblock_size = DEFAULT_CACHE_BLOCK_SIZE;
static int cache_dblock_size = DEFAULT_CACHE_BLOCK_SIZE;
static int cache_iwords_per_block = DEFAULT_CACHE_BLOCK_SIZE / WORD_SIZE;
static int cache_dwords_per_block = DEFAULT_CACHE_BLOCK_SIZE / WORD_SIZE;
static int cache_iassoc = DEFAULT_CACHE_ASSOC;
static int cache_dassoc = DEFAULT_CACHE_ASSOC;
static int cache_writeback = DEFAULT_CACHE_WRITEBACK;
static int cache_writealloc = DEFAULT_CACHE_WRITEALLOC;
static int cache_iperf = DEFAULT_CACHE_PERFECT;
static int cache_dperf = DEFAULT_CACHE_PERFECT;
static cache_replacement cache_dreplacement = DEFAULT_CACHE_REPLACEMENT;
static cache_replacement cache_ireplacement = DEFAULT_CACHE_REPLACEMENT;
/* cache model data structures */
static Pcache icache;
static Pcache dcache;
static cache c1;
static cache c2;
cache_stat cache_stat_inst;
cache_stat cache_stat_data;

/************************************************************/
void set_cache_param(param, value) cache_param param;
int value;
{
  switch (param)
  {
    case CACHE_PARAM_IBLOCK_SIZE:
      cache_iblock_size = value;
      cache_iwords_per_block = value / WORD_SIZE;
      break;
    case CACHE_PARAM_DBLOCK_SIZE:
      cache_dblock_size = value;
      cache_dwords_per_block = value / WORD_SIZE;
      break;
    case CACHE_PARAM_IASSOC:
      cache_iassoc = value;
      break;
    case CACHE_PARAM_DASSOC:
      cache_dassoc = value;
      break;
    case CACHE_PARAM_IPERF:
      cache_iperf = value;
      break;
    case CACHE_PARAM_DPERF:
      cache_dperf = value;
      break;
    case CACHE_PARAM_BLOCK_SIZE:
      cache_iblock_size = value;
      cache_iwords_per_block = value / WORD_SIZE;
      cache_dblock_size = value;
      cache_dwords_per_block = value / WORD_SIZE;
      break;
    case CACHE_PARAM_USIZE:
      cache_split = FALSE;
      cache_usize = value;
      break;
    case CACHE_PARAM_ISIZE:
      cache_split = TRUE;
      cache_isize = value;
      break;
    case CACHE_PARAM_DSIZE:
      cache_split = TRUE;
      cache_dsize = value;
      break;
    case CACHE_PARAM_ASSOC:
      cache_dassoc = value;
      cache_iassoc = value;
      break;
    case CACHE_PARAM_WRITEBACK:
      cache_writeback = TRUE;
      break;
    case CACHE_PARAM_WRITETHROUGH:
      cache_writeback = FALSE;
      break;
    case CACHE_PARAM_WRITEALLOC:
      cache_writealloc = TRUE;
      break;
    case CACHE_PARAM_NOWRITEALLOC:
      cache_writealloc = FALSE;
      break;
    case CACHE_PARAM_DREPLACEMENT:
      cache_dreplacement = LRU_POLICY;
      break;
    case CACHE_PARAM_IREPLACEMENT:
      cache_ireplacement = LRU_POLICY;
      break;
    default:
      printf("error set_cache_param: bad parameter value\n");
      exit(-1);
  }
}
/************************************************************/

/************************************************************/
void init_cache_params(Pcache c, int cache_size, int block_size, int wpb,
                       int assoc)
{
  c->size = (cache_size) / WORD_SIZE;
  c->associativity = assoc;
  unsigned num_sets = (c->size) / (c->associativity * wpb);
  int num_bits = LOG2(num_sets);
  c->n_sets = 1 << (num_bits);
  c->index_mask_offset = LOG2(block_size);
  c->tag_mask_offset = ((c->index_mask_offset) + num_bits);
  c->index_mask = (num_sets - 1) << (c->index_mask_offset);
  c->tag_mask = ((unsigned)(1 << (32 - (c->tag_mask_offset))) - 1)
                << (c->tag_mask_offset);
  c->set = (Pcache_set *)calloc(c->n_sets, sizeof(Pcache_set));
}

/************************************************************/

/************************************************************/
// Assuming every quantity to be in power of 2.
void init_cache()
{
  /* initialize the cache, and cache statistics data structures */

  /* Setting all fields zero */
  memset(&c1, 0, sizeof(cache));
  memset(&c2, 0, sizeof(cache));
  memset(&cache_stat_inst, 0, sizeof(cache_stat));
  memset(&cache_stat_data, 0, sizeof(cache_stat));

  if (cache_split == 0)
  {
    // In case there is no split both icache and dcache point to same cache.
    icache = &c1;
    dcache = &c1;
    init_cache_params(&c1, cache_usize, cache_dblock_size,
                      cache_dwords_per_block, cache_dassoc);
  }
  else
  {
    icache = &c1;
    dcache = &c2;
    init_cache_params(&c1, cache_isize, cache_iblock_size,
                      cache_iwords_per_block, cache_iassoc);
    init_cache_params(&c2, cache_dsize, cache_dblock_size,
                      cache_dwords_per_block, cache_dassoc);
  }
}
/************************************************************/

/************************************************************/
void perform_load(Pcache _cache, int index, int tag, Pcache_stat stat, int wpb,
                  int perf)
{
  Pcache_set set = (_cache->set)[index];
  if (set == NULL)
  {
    (_cache->set)[index] = (Pcache_set)calloc(1, sizeof(cache_set));
    set = (_cache->set)[index];
  }
  int prev_set_count = set->set_contents_count;
  (stat->accesses)++;

  // Execute rest only when cache is not perfect
  if (!perf)
  {
    int mem_access = lru_operation(set, tag, 1, _cache->associativity);

    if (mem_access)
    {
      // fetching words_per_block number of blocks from memory
      (stat->demand_fetches) += wpb;

      // number of memory access
      (stat->num_mem_access) += mem_access;

      (stat->misses)++;

      // writing words_per_block number of words to memory
      // if replacement of block whose dirty bit was 1 is done
      (stat->copies_back) += (wpb) * (mem_access - 1);
      // indicates replace
      if ((set->set_contents_count) == prev_set_count)
      {
        (stat->replacements)++;
      }
    }
  }
}

void perform_store(Pcache _cache, int index, int tag, Pcache_stat stat, int wpb,
                   int perf)
{
  Pcache_set set = (_cache->set)[index];
  if (set == NULL)
  {
    (_cache->set)[index] = (Pcache_set)calloc(1, sizeof(cache_set));
    set = (_cache->set)[index];
  }

  (stat->accesses)++;

  // Execute rest only when cache is not perfect
  if (!perf)
  {
    int prev_set_count = set->set_contents_count;
    int mem_access =
        lru_operation(set, tag, cache_writealloc, _cache->associativity);
    fflush(stdout);
    if (cache_writeback)
    {
      if (cache_writealloc)
      {
        /**
        on hits it writes to cache setting “dirty” bit for the block, main
        memory is not updated;
        EXPERIMENTAL: on miss fetch from main memory and update in cache,
        dont update in main memory.
        NOT: on misses it updates the block in main memory and brings the
        block to the cache;
        **/
        if (mem_access)
        {
          // represents a miss
          (stat->misses)++;

          // number of memory access
          (stat->num_mem_access) += mem_access;

          // fetching words_per_block number of blocks from memory
          (stat->demand_fetches) += wpb;

          // writing words_per_block number of words to memory
          // if replacement of block whose dirty bit was 1 is done
          (stat->copies_back) += (wpb) * (mem_access - 1);

          // indicates replace
          if ((set->set_contents_count) == prev_set_count)
          {
            (stat->replacements)++;
          }
          (set->head)->dirty = 1;
        }
        else
        {
          // represents a hit
          (set->head)->dirty = 1;
        }
      }
      else
      {
        /**
        on hits it writes to cache setting “dirty” bit for the block, main
        memory
        is not updated;
        on misses it updates the block in main memory not bringing that
        block to
        the cache;
        **/
        // In this case mem_access = 1 denotes there was a miss
        if (mem_access)
        {
          // number of memory access
          (stat->num_mem_access) += 1;

          (stat->misses)++;
          // writing 1 word to memory
          (stat->copies_back) += 1;
        }
        else
        {
          // represents a hit
          (set->head)->dirty = 1;
        }
      }
    }
    else
    {
      // writing 1 word to memory
      (stat->copies_back)++;
      if (cache_writealloc)
      {
        /**
        on hits it writes to cache and main memory
        on misses it updates the block in main memory and brings the block
        to the
        cache
        **/
        // adding one to num_mem_access for write
        (stat->num_mem_access) += 1;

        if (mem_access)
        {
          (stat->misses)++;

          // number of memory access (one to fetch from memory)
          (stat->num_mem_access) += 1;

          // fetching words_per_block number of blocks from memory
          (stat->demand_fetches) += wpb;

          if ((set->set_contents_count) == prev_set_count)
          {
            (stat->replacements)++;
          }
        }
      }
      else
      {
        /**
        on hits it writes to cache and main memory;
        on misses it updates the block in main memory not bringing that
        block to
        the cache;
        **/

        // adding one to num_mem_access for write
        (stat->num_mem_access) += 1;

        // In this case mem_access = 1 denotes there was a miss
        if (mem_access) (stat->misses)++;
      }
    }
  }
}

/************************************************************/

/************************************************************/
void perform_access(addr, access_type) unsigned addr, access_type;
{
  /* Assuming Unified cache for now */
  int index, tag;
  /* handle an access to the cache */
  switch (access_type)
  {
    case TRACE_INST_LOAD:
      index = (addr & (icache->index_mask)) >> (icache->index_mask_offset);
      tag = (addr & (icache->tag_mask)) >> (icache->tag_mask_offset);
      perform_load(icache, index, tag, &cache_stat_inst, cache_iwords_per_block,
                   cache_iperf);
      break;
    case TRACE_DATA_LOAD:
      index = (addr & (dcache->index_mask)) >> (dcache->index_mask_offset);
      tag = (addr & (dcache->tag_mask)) >> (dcache->tag_mask_offset);
      perform_load(dcache, index, tag, &cache_stat_data, cache_dwords_per_block,
                   cache_dperf);
      break;
    case TRACE_DATA_STORE:
      index = (addr & (dcache->index_mask)) >> (dcache->index_mask_offset);
      tag = (addr & (dcache->tag_mask)) >> (dcache->tag_mask_offset);
      perform_store(dcache, index, tag, &cache_stat_data,
                    cache_dwords_per_block, cache_dperf);
      break;
    default:
      printf("skipping access, unknown type(%d)\n", access_type);
  }
}
/************************************************************/

/************************************************************/
void write_dirty(Pcache _cache, Pcache_stat stat, int wpb)
{
  int i = 0;
  for (i = 0; i < (_cache->n_sets); i++)
  {
    Pcache_set set = (_cache->set)[i];
    if (set == NULL) continue;
    Pcache_line node = set->head;
    while (node != NULL)
    {
      if (node->dirty == 1)
      {
        node->dirty = 0;
        // writing words_per_block number of words to memory
        (stat->copies_back) += wpb;
      }
      node = node->LRU_next;
    }
  }
}
/************************************************************/

/************************************************************/
void flush()
{ /* flush the cache */
  // if (cache_split == 0)
  // {
  //   write_dirty(dcache, &cache_stat_data);
  // }
  // else
  // {
  //   write_dirty(icache, &cache_stat_inst);
  //   write_dirty(dcache, &cache_stat_data);
  // }
  write_dirty(dcache, &cache_stat_data, cache_dwords_per_block);
}
/************************************************************/

/************************************************************/
void delete (head, tail, item)Pcache_line *head, *tail;
Pcache_line item;
{
  if (item->LRU_prev)
  {
    item->LRU_prev->LRU_next = item->LRU_next;
  }
  else
  {
    /* item at head */
    *head = item->LRU_next;
  }

  if (item->LRU_next)
  {
    item->LRU_next->LRU_prev = item->LRU_prev;
  }
  else
  {
    /* item at tail */
    *tail = item->LRU_prev;
  }
}
/************************************************************/

/************************************************************/
/* inserts at the head of the list */
void insert(head, tail, item) Pcache_line *head, *tail;
Pcache_line item;
{
  item->LRU_next = *head;
  item->LRU_prev = (Pcache_line)NULL;

  if (item->LRU_next)
    item->LRU_next->LRU_prev = item;
  else
    *tail = item;

  *head = item;
}

/************************************************************/

/************************************************************/
void dump_settings()
{
  printf("*** CACHE SETTINGS ***\n");
  if (cache_split)
  {
    printf("  Split I- D-cache\n");
    printf("  I-cache size: \t%d\n", cache_isize);
    printf("  D-cache size: \t%d\n", cache_dsize);
  }
  else
  {
    printf("  Unified I- D-cache\n");
    printf("  Size: \t%d\n", cache_usize);
  }
  printf("  Associativity: \t%d\n", cache_dassoc);
  printf("  Block size: \t%d\n", cache_dblock_size);
  printf("  Write policy: \t%s\n",
         cache_writeback ? "WRITE BACK" : "WRITE THROUGH");
  printf("  Allocation policy: \t%s\n",
         cache_writealloc ? "WRITE ALLOCATE" : "WRITE NO ALLOCATE");
}
/************************************************************/

/************************************************************/
void print_stats()
{
  printf("\n*** CACHE STATISTICS ***\n");

  printf(" INSTRUCTIONS\n");
  printf("  accesses:  %d\n", cache_stat_inst.accesses);
  printf("  misses:    %d\n", cache_stat_inst.misses);
  if (!cache_stat_inst.accesses)
    printf("  miss rate: 0 (0)\n");
  else
    printf(
        "  miss rate: %2.4f (hit rate %2.4f)\n",
        (float)cache_stat_inst.misses / (float)cache_stat_inst.accesses,
        1.0 - (float)cache_stat_inst.misses / (float)cache_stat_inst.accesses);
  printf("  replace:   %d\n", cache_stat_inst.replacements);

  printf(" DATA\n");
  printf("  accesses:  %d\n", cache_stat_data.accesses);
  printf("  misses:    %d\n", cache_stat_data.misses);
  if (!cache_stat_data.accesses)
    printf("  miss rate: 0 (0)\n");
  else
    printf(
        "  miss rate: %2.4f (hit rate %2.4f)\n",
        (float)cache_stat_data.misses / (float)cache_stat_data.accesses,
        1.0 - (float)cache_stat_data.misses / (float)cache_stat_data.accesses);
  printf("  replace:   %d\n", cache_stat_data.replacements);

  printf(" TRAFFIC (in words)\n");
  printf("  demand fetch:  %d\n",
         (cache_stat_inst.demand_fetches + cache_stat_data.demand_fetches));
  printf("  copies back:   %d\n",
         (cache_stat_inst.copies_back + cache_stat_data.copies_back));
}
/************************************************************/

/************************************************************/
void print_eval_stats()
{
  assert(cache_isize == cache_dsize);
  assert(cache_iblock_size == cache_dblock_size);
  assert(cache_iassoc == cache_dassoc);
  int cs = cache_isize;
  int assoc = cache_iassoc;
  char *ivd = (cache_split == 0) ? "Unified" : "Split";
  int bs = cache_iblock_size;
  char *write = (cache_writeback == 0) ? "WT" : "WB";
  char *alloc = (cache_writealloc == 0) ? "WNA" : "WA";

  float hit_rate_inst =
      (1.0 - (float)cache_stat_inst.misses / (float)cache_stat_inst.accesses);
  float hit_rate_data =
      (1.0 - (float)cache_stat_data.misses / (float)cache_stat_data.accesses);

  printf("|| %-8d | %-8s | %-4d | %-8d |  %-4s |  %-4s || ", cs, ivd, bs, assoc,
         write, alloc);
  // Instruction
  // Misses
  printf(" %-8d | ", cache_stat_inst.misses);
  // Repl
  printf("%-8d | ", cache_stat_inst.replacements);
  // Hit Rate
  printf(" %-7.4f | ", hit_rate_inst);

  // Data
  // Misses
  printf("%-8d | ", cache_stat_data.misses);
  // Repl
  printf("%-8d | ", cache_stat_data.replacements);
  // Hit Rate
  printf(" %-7.4f | ", hit_rate_data);

  // Total
  // Demand Fetch
  printf("%-8d | ",
         (cache_stat_inst.demand_fetches + cache_stat_data.demand_fetches));
  // Copies Back
  printf("%-8d ||\n",
         (cache_stat_inst.copies_back + cache_stat_data.copies_back));
}
/************************************************************/
