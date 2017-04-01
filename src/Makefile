CC = gcc
DEBUG = -g
OBJDIR = obj
HEADER_DIR = include
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = "-lpthread"
INCLUDE = -I $(HEADER_DIR)
HEADERS = $(wildcard $(HEADER_DIR)/*.h)
SRCS = main.c global_vars.c utils.c parser.c instruction_fetch.c register_read.c thread_alu.c data_memory.c register_write.c display.c svg.c
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

simulator: $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

debug: CFLAGS += -DDEBUG
debug: LFLAGS += -DDEBUG
debug: simulator

time: CFLAGS += -DTIME
time: LFLAGS += -DTIME
time: simulator

$(OBJDIR)/%.o: %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean


clean:
	rm -rf ./$(OBJDIR)
