#ifndef LRU_H
#define LRU_H

#include "cache.h"

Pcache_line constructor_Pcache_line(unsigned tag);
int lru_operation(Pcache_set set, unsigned tag, int allocate, int assoc);
#endif