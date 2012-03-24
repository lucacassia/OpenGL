CC=g++
CFLAGS=-lglut -lGL -lGLU -lX11 -lm
SOURCES=Conway.cc life.cpp
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=life

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $< -o $@

clean:
	rm -f *.o *.gch *~ $(EXECUTABLE)

