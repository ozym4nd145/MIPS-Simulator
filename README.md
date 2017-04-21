# MIPS Pipelined Processor Simulator.
#### *Submitted by - Ankesh Gupta (2015CS10435), Suyash Agrawal (2015CS10262)*

### Usage
* Run `make` to compile the source code.
* ` Usage: processor_simulator <path_input_HEX_file> <input_of_cfg_file> <path_output_SVG_file> <path_output_result_file> `
* Run `make cache` to compile the trace cache simulator
* Open the svg file create on Chrome Browser
* Type `step` to advanced a single clock cycle
* Type `memdump <start_mem_addr> <num_block>` to view memory dump
* Type `regdump` to view register dump

### Notes
* SVG refreshes itself in an interval of 0.5 sec

### Assumptions
* The program terminates assuming that the last read instruction traverses through all stages of pipeline.
* Whenever a branch statement comes, following instructions are read in anticipation of branch not been taken. Should a branch be taken, the read instructions are flushed.
* Instruction Count is count of instructions that gets completely executed, meaning that they have traversed through all stages of the pipeline.
* Cycles denote the cycle count for which the processor simulation was running. Each step corresponds to a cycle.
* L1-I cache access are counted assuming that all Instruction that are fetched are in Cache.
* L1-D cache access are counted assuming that whole Data-Memory is in Cache.
