simulator: main.o global_vars.o utils.o parser.o instruction_fetch.o register_read.o thread_alu.o data_memory.o register_write.o display.o svg.o
	gcc -g -Wall -o simulator main.o global_vars.o utils.o parser.o instruction_fetch.o register_read.o thread_alu.o data_memory.o register_write.o display.o svg.o -lpthread
main.o: main.c
	gcc -c main.c
utils.o: utils.c
	gcc -c utils.c
parser.o: parser.c
	gcc -c parser.c
global_vars.o: global_vars.c
	gcc -c global_vars.c
instruction_fetch.o: instruction_fetch.c
	gcc -c instruction_fetch.c
register_read.o: register_read.c
	gcc -c register_read.c
thread_alu.o: thread_alu.c
	gcc -c thread_alu.c
data_memory.o: data_memory.c
	gcc -c data_memory.c
register_write.o: register_write.c
	gcc -c register_write.c
display.o: display.c
	gcc -c display.c
svg.o: svg.c
	gcc -c svg.c
clean:
	rm -f ./*.o
