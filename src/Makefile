CC = gcc
INCLUDE = -I "./include"
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = "-lpthread"

HEADERS = $(wildcard include/*.h)
SRCS = main.c global_vars.c utils.c parser.c instruction_fetch.c register_read.c thread_alu.c data_memory.c register_write.c display.c svg.c
OBJ_DIR = obj
OBJS = ./$(OBJ_DIR)/$(SRCS:.c=.o)

simulator: $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

./$(OBJ_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -f ./*.o
