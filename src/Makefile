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

processor_simulator: $(OBJS)
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

submit: processor_simulator
	rm -rf ../ignore/Assignment5 ../ignore/*.zip
	mkdir -p ../ignore/Assignment5/src
	cp -r $(SRCS) $(HEADER_DIR) Makefile ../ignore/Assignment5/src
	cp ../README.md processor_simulator ../results.txt ../ignore/Assignment5
	cd ../ignore/Assignment5 &&	zip -r ../2015CS10262_Assignment5.zip ./*
	cp ../ignore/2015CS10262_Assignment5.zip ../ignore/2015CS10435_Assignment5.zip
	rm -rf ../ignore/Assignment5