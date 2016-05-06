# vim: noet

CC = gcc
CFLAGS = -g -std=c11 -MMD
LIB = -lm -ltiff
EXEC = bin/lmtool
SRCS = $(wildcard src/*.c)
OBJS = $(addprefix build/,$(notdir $(SRCS:.c=.o)))

default: directories all

all: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LIB)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(LIB)

directories:
	mkdir -p bin build

clean:
	-@rm build/*.o build/*.d $(EXEC) 2>/dev/null || true
