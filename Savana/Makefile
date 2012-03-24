CC=g++
CFLAGS=-lglut -lGL -lGLU -lX11 -lm
SOURCES=../lib/point.cc SAVANA.cpp
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=savana

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $< -o $@

clean:
	rm -f *.o *.gch *~ $(EXECUTABLE) ../lib/*.o

