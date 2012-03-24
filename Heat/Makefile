CC=g++
CFLAGS=-lglut -lGL -lGLU -lX11 -lm
SOURCES=../lib/complex.cc ../lib/Timer.cc sheet.cc Heat.cpp
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=heat

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $< -o $@

clean:
	rm -f *.o *.gch *~ $(EXECUTABLE) ../lib/*.o

