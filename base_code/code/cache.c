/*
 * cache.c
 */

#include "cache.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru.h"

#include "main.h"

/* cache configuration parameters */
static int cache_split = 0;
static int cache_usize = DEFAULT_CACHE_SIZE;
static int cache_isize = DEFAULT_CACHE_SIZE;
static int cache_dsize = DEFAULT_CACHE_SIZE;
static int cache_block_size = DEFAULT_CACHE_BLOCK_SIZE;
static int words_per_block = DEFAULT_CACHE_BLOCK_SIZE / WORD_SIZE;
int cache_assoc = DEFAULT_CACHE_ASSOC;
static int cache_writeback = DEFAULT_CACHE_WRITEBACK;
static int cache_writealloc = DEFAULT_CACHE_WRITEALLOC;

/* cache model data structures */
static Pcache icache;
static Pcache dcache;
static cache c1;
static cache c2;
static cache_stat cache_stat_inst;
static cache_stat cache_stat_data;

/************************************************************/
void set_cache_param(param, value) int param;
int value;
{
  switch (param)
  {
    case CACHE_PARAM_BLOCK_SIZE:
      cache_block_size = value;
      words_per_block = value / WORD_SIZE;
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
      cache_assoc = value;
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
    default:
      printf("error set_cache_param: bad parameter value\n");
      exit(-1);
  }
}
/************************************************************/

/************************************************************/
void init_cache_params(Pcache c, int cache_size)
{
  c->size = (cache_size) / WORD_SIZE;
  c->associativity = cache_assoc;
  unsigned num_sets = (c->size) / (c->associativity * words_per_block);
  int num_bits = LOG2(num_sets);
  c->n_sets = 1 << (num_bits);
  c->index_mask_offset = LOG2(cache_block_size);
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
    init_cache_params(&c1, cache_usize);
  }
  else
  {
    icache = &c1;
    dcache = &c2;
    init_cache_params(&c1, cache_isize);
    init_cache_params(&c2, cache_dsize);
  }
}
/************************************************************/

/************************************************************/
void perform_load(Pcache _cache, int index, int tag, Pcache_stat stat)
{
  Pcache_set set = (_cache->set)[index];
  if (set == NULL)
  {
    (_cache->set)[index] = (Pcache_set)calloc(1, sizeof(cache_set));
    set = (_cache->set)[index];
  }
  int prev_set_count = set->set_contents_count;
  int mem_access = lru_operation(set, tag, 1);
  (stat->accesses)++;
  if (mem_access)
  {
    // +2 if dirty bit was 1 else +1
    (stat->demand_fetches)++;
    (stat->misses)++;

    // +1 if replacement of block whose dirty bit was 1
    (stat->copies_back) += (mem_access - 1);

    // indicates replace
    if ((set->set_contents_count) == prev_set_count)
    {
      (stat->replacements)++;
    }
  }
}

void perform_store(Pcache _cache, int index, int tag, Pcache_stat stat)
{
  Pcache_set set = (_cache->set)[index];
  if (set == NULL)
  {
    (_cache->set)[index] = (Pcache_set)calloc(1, sizeof(cache_set));
    set = (_cache->set)[index];
  }

  (stat->accesses)++;

  if (cache_writeback)
  {
    int prev_set_count = set->set_contents_count;
    int mem_access = lru_operation(set, tag, 1);
    (set->head)->dirty = 1;

    if (mem_access)
    {
      (stat->misses)++;
      (stat->demand_fetches)++;
      (stat->copies_back) += (mem_access - 1);
      // indicates replace
      if ((set->set_contents_count) == prev_set_count)
      {
        (stat->replacements)++;
      }
    }
  }
  else
  {
    int prev_set_count = set->set_contents_count;
    int mem_access = 0;
    if (cache_writealloc)
    {
      mem_access = lru_operation(set, tag, 1);
      if (mem_access)
      {
        (stat->misses)++;
        (stat->demand_fetches)++;
        if ((set->set_contents_count) == prev_set_count)
        {
          (stat->replacements)++;
        }
      }
    }
    else
    {
      int miss = lru_operation(set, tag, 0);
      if (miss) (stat->misses)++;
    }

    (stat->copies_back)++;
    (stat->accesses)++;
  }
}

/************************************************************/

/************************************************************/
void perform_access(addr, access_type) unsigned addr, access_type;
{
  /* Assuming Unified cache for now */
  int index = (addr & (icache->index_mask)) >> (icache->index_mask_offset);
  int tag = (addr & (icache->tag_mask)) >> (icache->tag_mask_offset);
  /* handle an access to the cache */
  switch (access_type)
  {
    case TRACE_INST_LOAD:
      perform_load(icache, index, tag, &cache_stat_inst);
      break;
    case TRACE_DATA_LOAD:
      perform_load(dcache, index, tag, &cache_stat_data);
      break;
    case TRACE_DATA_STORE:
      perform_store(dcache, index, tag, &cache_stat_data);
      break;
    default:
      printf("skipping access, unknown type(%d)\n", access_type);
  }
}
/************************************************************/

/************************************************************/
void write_dirty(Pcache _cache, Pcache_stat stat)
{
  int i = 0;
  for (i = 0; i < (_cache->n_sets); i++)
  {
    Pcache_set set = (icache->set)[i];
    if (set == NULL) continue;
    Pcache_line node = set->head;
    while (node != NULL)
    {
      if (node->dirty == 1)
      {
        node->dirty = 0;
        stat->copies_back += 1;
      }
      node = node->LRU_next;
    }
  }
}
/************************************************************/

/************************************************************/
void flush()
{ /* flush the cache */
  if (cache_split == 0)
  {
    write_dirty(dcache, &cache_stat_data);
  }
  else
  {
    write_dirty(icache, &cache_stat_inst);
    write_dirty(dcache, &cache_stat_data);
  }
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
  printf("  Associativity: \t%d\n", cache_assoc);
  printf("  Block size: \t%d\n", cache_block_size);
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
         cache_stat_inst.demand_fetches + cache_stat_data.demand_fetches);
  printf("  copies back:   %d\n",
         cache_stat_inst.copies_back + cache_stat_data.copies_back);
}
/************************************************************/
