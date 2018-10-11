# compilateur
CC := gcc
# options de compilation
CFLAGS := -std=c99 -Wall -Wextra -pedantic -ggdb

# règle de compilation --- exécutables
all : main

main : main.o noeud.o huffman.o
	$(CC) $(CFLAGS) -o $@ $^ -lm	

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< -lm

# options de compilation
clean:
	rm *.o main 
