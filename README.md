Assumptions:

1. The program terminates assuming that the last read instruction traverses through all stages of pipeline.
2. Whenever a branch statement comes, following instructions are read in anticipation of branch not been taken. Should a branch be taken, the read instructions are flushed.
3. Instruction Count is count of instructions that gets completely executed, meaning that they have traversed through all stages of the pipeline.
4. Cycles denote the cycle count for which the processor simulation was running. Each step corresponds to a cycle.
5. L1-I cache access are counted assuming that all Instruction that are fetched are in Cache.
6. L1-D cache access are counted assuming that whole Data-Memory is in Cache.
