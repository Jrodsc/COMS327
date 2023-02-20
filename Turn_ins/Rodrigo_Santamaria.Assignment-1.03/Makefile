CC 			= gcc
CFLAGS 		= -Wall -Werror -ggdb
OBJFILES	= tlib.o priority_queue.o  queue.o map.o main.o
TARGET 		= PokemonGame

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean: 
	rm -f $(OBJFILES) $(TARGET) *~ core
