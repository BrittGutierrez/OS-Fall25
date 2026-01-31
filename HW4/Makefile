# Simple Makefile for CS370 HW4 (Python version)
PYTHON = python3
SCRIPT = scheduler.py
FILE   = processinfo.csv
QUANTUM = 3
ZIPNAME = Brittany-Gutierrez-HW4.zip

# Default target: run the program
run:
	$(PYTHON) $(SCRIPT) $(FILE) $(QUANTUM)

# Quick test using a small built-in example file
test:
	echo "ProcessID,Arrival Time,Burst Time" > sample.csv
	echo "3,0,6" >> sample.csv
	echo "2,0,5" >> sample.csv
	echo "1,12,8" >> sample.csv
	echo "4,13,6" >> sample.csv
	$(PYTHON) $(SCRIPT) sample.csv 3

# Create submission zip (only .py and README.txt)
zip:
	rm -f $(ZIPNAME)
	zip -q $(ZIPNAME) *.py README.txt
	@echo "Created $(ZIPNAME). Please unzip once to verify before uploading."

# Clean up helper/test files
clean:
	rm -f *.zip sample.csv
