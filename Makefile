CFLAGS := -Wall

all: strogatz

strogatz: src/main.c src/graph.c src/graph.h src/electeurs.c src/electeurs.h Makefile
	gcc $(CFLAGS) -o $@ $(filter %.c,$^)
