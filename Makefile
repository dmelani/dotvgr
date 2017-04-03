CFLAGS=-g -Wall -Wextra -std=c11 -I/usr/local/Cellar/graphviz/2.40.1/include
LDFLAGS=-lcgraph -L/usr/local/Cellar/graphviz/2.40.1/lib
PROG=dotvgr

SRCS = src/dotvgr.c
OBJS = $(SRCS:.c=.o)


all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(PROG) $(OBJS)

.PHONY: all clean
