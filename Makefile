CC=g++
CFLAGS=-std=c++0x -c -Wall
LDFLAGS=-lboost_date_time
SOURCES=test.cpp logger.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f test.o logger.o test

.PHONY: all clean
