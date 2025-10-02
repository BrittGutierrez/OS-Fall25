CC = gcc
CFLAGS = -Wall -g -std=c99
TARGET = Driver
SOURCES = Driver.c MemoryManager.c
OBJECTS = $(SOURCES:.c=.o)
ZIPFILE = project_files.zip

# The 'all' target now depends on both the executable and the zip file
all: $(TARGET) $(ZIPFILE)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Direct rule for creating the zip file
$(ZIPFILE): $(SOURCES) MemoryManager.h Makefile
	zip $@ $^

Driver.o: Driver.c MemoryManager.h
	$(CC) $(CFLAGS) -c Driver.c

MemoryManager.o: MemoryManager.c MemoryManager.h
	$(CC) $(CFLAGS) -c MemoryManager.c

clean:
	rm -f $(TARGET) $(OBJECTS) $(ZIPFILE)

# Keep the zip target for manual use
zip: $(ZIPFILE)

.PHONY: all clean zip