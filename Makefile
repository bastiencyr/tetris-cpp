CC=g++
CFLAGS= $(shell sdl2-config --cflags)
LDFLAGS=  $(shell sdl2-config --libs) -lSDL2_mixer 
#-lSDL2_ttf

TARGET = tetris

INCLUDE_PATH = include
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Linking complete!"

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(TARGET)

#all: $(EXEC)


#tetris: tetris.o func.o
#	$(CC) -o $@ $^ $(LDFLAGS)

#tetris.o: $(SRC)/tetris.cpp $(HEADERS)/func.hpp
#	$(CC) -o $@ -c $< $(CFLAGS)

#func.o: $(SRC)/func.cpp $(HEADERS)/func.hpp
#	$(CC) -o $@ -c $< $(CFLAGS)
