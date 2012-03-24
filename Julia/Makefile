CC=g++
CFLAGS=-lglut -lGL -lGLU -lX11 -fopenmp -lm
SOURCES=../lib/complex.cc ../lib/Timer.cc Julia.cc JULIA.cpp
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=julia

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $< -o $@

clean:
	rm -f *.o *.gch *~ $(EXECUTABLE) ../lib/*.o

