CC 			= gcc
CFLAGS 		= -Wall -Werror -ggdb -lncurses
OBJFILES	= player.o tlib.o priority_queue.o queue.o map.o world.o main.o
TARGET 		= PokemonGame

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean: 
	rm -f $(OBJFILES) $(TARGET) *~ core
