#ifndef LRU_H
#define LRU_H

#include "cache.h"

int lru_operation(Pcache_set set, unsigned tag, int allocate);
#endif