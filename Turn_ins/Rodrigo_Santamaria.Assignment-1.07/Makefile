CPP 		= g++
CPPFLAGS 	= -Wall -Werror -ggdb
LDFLAGS 	= -lncurses
BIN_NAME    = PokemonGame

SRC 		= src
BIN 		= bin
OBJ 		= build
INC 		= -I include

OBJFILES 	= data_classes.o player.o tlib.o priority_queue.o queue.o map.o world.o parsing.o main.o
SRCFILES 	= data_classes.cpp player.cpp tlib.cpp priority_queue.cpp queue.cpp map.cpp world.cpp parsing.cpp main.cpp

BIN_FILES 	= $(BIN_NAME:%=$(BIN)/%)
OBJFILES 	= $(SRCFILES:%.cpp=$(OBJ)/%.o)

build: 		$(BIN_FILES)

clean:
	rm -f $(OBJFILES) $(BIN)/$(BIN_NAME) *~ core

.PHONY: build clean

$(BIN_FILES) : $(OBJFILES)
	$(CPP) -o $@ $^ $(LDFLAGS)

$(OBJFILES)  : $(OBJ)/%.o: $(SRC)/%.cpp 
	mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c -o $@ $< $(INC)

