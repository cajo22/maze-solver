CC = gcc
CFLAGS = -Wall -g
FILES = convert.c

maze-solver: $(FILES)
	mkdir -p build
	$(CC) $(CFLAGS) $(FILES) -o build/maze-solver

clean:
	rm -r build/

