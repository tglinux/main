CC=gcc
CXX=g++
BIN=test
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
CFLAGS=-Wall -finput-charset=GBK
LDFLGAS=-lpthread
INCS=
LIBS=

$(BIN):$(OBJS)
	$(CC) $^ -fexec-charset=UTF-8 -o $@ $(L DFLGAS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)
distclean:
	rm -f $(OBJS)
	rm -f $(BIN)
