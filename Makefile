CFLAGS=-g -Wall -Wextra -std=c11
LDFLAGS=-lcgraph
PROG=dotvgr

SRCS = src/dotvgr.c
OBJS = $(SRCS:.c=.o)


all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(PROG) $(OBJS)

.PHONY: all clean
