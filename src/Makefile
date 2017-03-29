CC = gcc
INCLUDE = -I "./include"
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = "-lpthread"

HEADERS = $(wildcard include/*.h)
SRCS = main.c global_vars.c utils.c parser.c instruction_fetch.c register_read.c thread_alu.c data_memory.c register_write.c display.c svg.c
OBJDIR = obj
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

simulator: $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean


clean:
	rm -rf ./$(OBJDIR)
