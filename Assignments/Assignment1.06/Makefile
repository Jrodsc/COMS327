CC 			= g++
CFLAGS 		= -Wall -Werror -ggdb 
LDFLAGS 	= -lncurses 
OBJFILES	= player.o tlib.o priority_queue.o queue.o map.o world.o main.o
TARGET 		= PokemonGame

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS) 2> error.log 

clean: 
	rm -f $(OBJFILES) $(TARGET) *~ core
