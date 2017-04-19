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
}

// Searching for the right block in cache line matching with tag

Pcache_line search(Pcache_line head, unsigned tag)
{
  while (head)
  {
    if (head->tag == tag) return head;  // search successful
    head = head->LRU->next;
  }
  return NULL;  // csearch unsuccessful
}

// lru_operation receives cache_set as parameter and updates the cache set.
// It performs its operation independent of read-write operations.

int lru_operation(Pcache_set set, unsigned tag, int allocate)
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
      if (set->set_content_count == cache_assoc)  //  set is Full | follow LRU
                                                  //  to remove the least
                                                  //  recently used
      {
        if (tail->dirty == 1) mem_access = 1;
        delete (set->head, set->tail, set->tail);
        insert(set->head, set->tail, constructor_Pache_line());
      }
      else  // set is unsaturated | simply fetch from memory
      {
        set->set_content_count++;
        insert(set->head, set->tail, constructor_Pache_line());
      }
    }

    else
      mem_access = 1;
  }
  return mem_access;
}
