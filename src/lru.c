#include "lru.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

// Constructor for Pcache_line

Pcache_line constructor_Pcache_line(unsigned tag)
{
  Pcache_line temp = (Pcache_line)malloc(sizeof(Pcache_line));
  temp->tag = tag;
  temp->dirty = 0;
  temp->LRU_next = (Pcache_line)malloc(sizeof(Pcache_line));
  temp->LRU_prev = (Pcache_line)malloc(sizeof(Pcache_line));
  return temp;
}

// Searching for the right block in cache line matching with tag

Pcache_line search(Pcache_line head, unsigned tag)
{
  while (head)
  {
    if (head->tag == tag) return head;  // search successful
    head = head->LRU_next;
  }
  return NULL;  // csearch unsuccessful
}

// lru_operation receives cache_set as parameter and updates the cache set.
// It performs its operation independent of read-write operations.

int lru_operation(Pcache_set set, unsigned tag, int allocate, int assoc)
{
  Pcache_line found = search(set->head, tag);

  int mem_access = 0;

  if (found)  // cache HIT
  {
    delete (&(set->head), &(set->tail), found);
    insert(&(set->head), &(set->tail), found);
  }

  else  // cache MISS
  {
    if (allocate == 1)
    {
      if (set->set_contents_count == assoc)  //  set is Full | follow LRU
                                             //  to remove the least
                                             //  recently used

      {
        if (set->tail->dirty == 1) mem_access++;
        delete (&(set->head), &(set->tail), set->tail);
        mem_access++;
        insert(&(set->head), &(set->tail), constructor_Pcache_line(tag));
      }
      else  // set is unsaturated | simply fetch from memory
      {
        mem_access++;
        set->set_contents_count++;
        insert(&(set->head), &(set->tail), constructor_Pcache_line(tag));
      }
    }

    else
      mem_access = 1;
  }
  return mem_access;
}
