CC = g++
CFLAGS = -c -Wall
SOURCES = main.cpp Parse.cpp SPNParser.cpp
OBJECTS = $(SOURCES:.cpp=.o)
PROGRAM = SPNParser
SUBPROGRAM = txttobin

all: $(SOURCES) $(PROGRAM) 
	
$(PROGRAM): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@