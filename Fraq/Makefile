CC=g++
CFLAGS=-lglut -lGL -lGLU -lX11 -lpthread -lm
SOURCES=../lib/complex.cc ../lib/Timer.cc Fraq.cc FRAQ.cpp
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=fraq

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $< -o $@

clean:
	rm -f *.o *.gch *~ $(EXECUTABLE) ../lib/*.o

