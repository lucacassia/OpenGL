CC=g++
CFLAGS=-lglut -lGL -lGLU -lX11 -lfftw3 -lm
SOURCES=WAVE.cpp
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=wave

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $< -o $@

clean:
	rm -f *.o *.gch *~ $(EXECUTABLE) ../lib/*.o

