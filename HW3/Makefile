CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lrt
TARGETS = Manager Palindrome

all: $(TARGETS)

Manager: Manager.o
	$(CC) $(CFLAGS) -o Manager Manager.o $(LDFLAGS)

Palindrome: Palindrome.o
	$(CC) $(CFLAGS) -o Palindrome Palindrome.o $(LDFLAGS)

clean:
	rm -f *.o $(TARGETS)

run: all
	./Manager racecar kayak apple abcba hello
