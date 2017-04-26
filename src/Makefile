CC = gcc
DEBUG = -g
OBJDIR = obj
HEADER_DIR = include
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = -lm -lpthread
INCLUDE = -I $(HEADER_DIR)
HEADERS = $(wildcard $(HEADER_DIR)/*.h)
SRCS = main.c global_vars.c utils.c parser.c instruction_fetch.c register_read.c thread_alu.c data_memory.c register_write.c display.c svg.c cache.c lru.c
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

processor_simulator: $(OBJDIR)/lex.parse_cfg.o $(OBJDIR)/parse_cfg.tab.o $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

cache: $(OBJDIR)/cache.o $(OBJDIR)/lru.o $(OBJDIR)/trace.o
	$(CC) $(LFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

eval: CFLAGS += -DEVALUATION
eval: LFLAGS += -DEVALUATION
eval: cache

debug: CFLAGS += -DDEBUG
debug: LFLAGS += -DDEBUG
debug: processor_simulator

time: CFLAGS += -DTIME
time: LFLAGS += -DTIME
time: processor_simulator

$(OBJDIR)/%.o: %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJDIR)/parse_cfg.tab.o: parse_cfg.y | $(OBJDIR)
	bison -p parse_cfg -d parse_cfg.y
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ parse_cfg.tab.c

$(OBJDIR)/lex.parse_cfg.o: parse_cfg.l $(OBJDIR)/parse_cfg.tab.o | $(OBJDIR)
	flex -P parse_cfg parse_cfg.l
	$(CC) $(CFLAGS) -o $@ lex.parse_cfg.c

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean

clean:
	rm -rf ./$(OBJDIR)
	rm -f ./*.tab.*
	rm -f ./lex.*

test_parse: $(OBJDIR)/parse_cfg.tab.o $(OBJDIR)/lex.parse_cfg.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(OBJDIR)/test_parse.o -c test_parse.c
	$(CC) $(LFLAGS) $(INCLUDE) -o $@ $(OBJDIR)/parse_cfg.tab.o $(OBJDIR)/lex.parse_cfg.o $(OBJDIR)/test_parse.o $(OBJDIR)/cache.o $(OBJDIR)/lru.o $(OBJDIR)/global_vars.o $(LIBS)


submit: processor_simulator
	rm -rf ../ignore/Assignment6 ../ignore/*.zip
	mkdir -p ../ignore/Assignment6/src
	cp -r ../src ../ignore/Assignment6/src
	cp ../README.md processor_simulator cache ../ignore/Assignment6
	cd ../ignore/Assignment6 &&	zip -r ../2015CS10262_Assignment6.zip ./*
	cp ../ignore/2015CS10262_Assignment6.zip ../ignore/2015CS10435_Assignment6.zip
	rm -rf ../ignore/Assignment6