
CC = gcc
CFLAGS = -Wall -g
TARGETS = Generator OddEven PerfectSquare Factorial

.PHONY: all clean run

all: $(TARGETS)

Generator: Generator.c
	$(CC) $(CFLAGS) -o Generator Generator.c

OddEven: OddEven.c
	$(CC) $(CFLAGS) -o OddEven OddEven.c -lm

PerfectSquare: PerfectSquare.c
	$(CC) $(CFLAGS) -o PerfectSquare PerfectSquare.c -lm

Factorial: Factorial.c
	$(CC) $(CFLAGS) -o Factorial Factorial.c

clean:
	rm -f *.o $(TARGETS)

run: all
	./Generator input.txt
