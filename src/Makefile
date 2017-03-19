simulator: main.o global_vars.o utils.o parser.o instruction_fetch.o register_read.o thread_alu.o data_memory.o register_write.o 
	gcc -g -Wall -o simulator main.o global_vars.o utils.o parser.o instruction_fetch.o register_read.o thread_alu.o data_memory.o register_write.o -lpthread
main.o:
	gcc -c main.c
utils.o:
	gcc -c utils.c
parser.o:
	gcc -c parser.c
global_vars.o:
	gcc -c global_vars.c
instruction_fetch.o:
	gcc -c instruction_fetch.c
register_read.o:
	gcc -c register_read.c
data_memory.o:
	gcc -c data_memory.c
register_write.o:
	gcc -c register_write.c
clean:
	rm ./*.o
