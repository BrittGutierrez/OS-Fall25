JAVAC = javac
JAVA  = java

SOURCES = Coordinator.java Buffer.java Producer.java Consumer.java
CLASSES = $(SOURCES:.java=.class)

# Default: compile everything
build: $(CLASSES)

%.class: %.java
	$(JAVAC) $<

# Run with a test seed and alphabetSeed (you can change these)
run: build
	$(JAVA) Coordinator 10 27

# Clean out compiled classes and any old archives
clean:
	rm -f *.class *.tar *.zip

# Create the tar file for submission (change name if needed)
tar: clean
	tar -cvf Brittany-Gutierrez-HW5.tar *.java Makefile
