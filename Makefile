CC=g++
CFLAGS= $(shell sdl2-config --cflags)
LDFLAGS=  $(shell sdl2-config --libs) 
#-lSDL2_ttf
EXEC=tetris
SRC=src
HEADERS=include

all: $(EXEC)

tetris: tetris.o func.o
	$(CC) -o $@ $^ $(LDFLAGS)

tetris.o: $(SRC)/tetris.cpp $(HEADERS)/func.hpp 
	$(CC) -o $@ -c $< $(CFLAGS)

func.o: $(SRC)/func.cpp $(HEADERS)/func.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

#positionInSpace.o: $(SRC)/positionInSpace.cpp $(HEADERS)/positionInSpace.hpp
#	$(CC) -o $@ -c $< $(CFLAGS)

#tetrimino.o: $(SRC)/tetrimino.cpp $(HEADERS)/tetrimino.hpp $(HEADERS)/positionInSpace.hpp
#	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
	rm tetris
