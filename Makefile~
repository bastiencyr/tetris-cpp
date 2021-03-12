CC=g++
CFLAGS= $(shell sdl2-config --cflags)
LDFLAGS=  $(shell sdl2-config --libs) -lSDL2_ttf
EXEC=tetris
SRC=src
HEADERS=src/headers

all: $(EXEC)

tetris: tetris.o utils.o positionInSpace.o tetrimino.o
	$(CC) -o $@ $^ $(LDFLAGS)

tetris.o: $(SRC)/tetris.cpp $(HEADERS)/utils.hpp $(HEADERS)/positionInSpace.hpp $(HEADERS)/tetrimino.hpp $(HEADERS)/constants.hpp 
	$(CC) -o $@ -c $< $(CFLAGS)

utils.o: $(SRC)/utils.cpp $(HEADERS)/utils.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

positionInSpace.o: $(SRC)/positionInSpace.cpp $(HEADERS)/positionInSpace.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

tetrimino.o: $(SRC)/tetrimino.cpp $(HEADERS)/tetrimino.hpp $(HEADERS)/positionInSpace.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
