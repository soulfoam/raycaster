EXEC = lowrez
CC = gcc

CFLAGS = -std=c99 -Wall -Wextra
LDFLAGS = -lm -lSDL2main -lSDL2
INCLUDES = -Isrc/includes

SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)

all: $(EXEC) $(OBJS) clean tags

${EXEC}: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES) $(CFLAGS)


.PHONY: clean soulfoam

tags:
	ctags -R .

clean:
	rm -rf src/*.o

soulfoam: clean
	rm -rf $(EXEC)
